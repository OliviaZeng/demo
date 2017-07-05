     上层要能使用select()和poll()系统调用来监测某个设备文件描述符，那么就必须实现这个设备驱动程序中struct file_operation结构体的poll函数，为什么？

因为这两个系统调用最终都会调用驱动程序中的poll函数来初始化一个等待队列项, 然后将其加入到驱动程序中的等待队列头，这样就可以在硬件可读写的时候wake up这个等待队列头，然后等待(可以是多个)同一个硬件设备可读写事件的进程都将被唤醒。

(这个等待队列头可以包含多个等待队列项，这些不同的等待队列项是由不同的应用程序调用select或者poll来监测同一个硬件设备的时候调用file_operation的poll函数初始化填充的)。

       下面就以select系统调用分析具体实现，源码路径：fs/select.c。

      

一、          select()系统调用代码走读

调用顺序如下：sys_select() à core_sys_select() à do_select() à fop->poll()

 

SYSCALL_DEFINE5(select, int, n, fd_set __user *, inp, fd_set __user *, outp,

fd_set __user *, exp, struct timeval __user *, tvp)

{

       struct timespec end_time, *to = NULL;

       struct timeval tv;

       int ret;

 

       if (tvp) {// 如果超时值非NULL

              if (copy_from_user(&tv, tvp, sizeof(tv)))   // 从用户空间取数据到内核空间

                     return -EFAULT;

 

              to = &end_time;

              // 得到timespec格式的未来超时时间

              if (poll_select_set_timeout(to,

                            tv.tv_sec + (tv.tv_usec / USEC_PER_SEC),

                            (tv.tv_usec % USEC_PER_SEC) * NSEC_PER_USEC))

                     return -EINVAL;

       }

 

       ret = core_sys_select(n, inp, outp, exp, to);             // 关键函数

       ret = poll_select_copy_remaining(&end_time, tvp, 1, ret);

       /*如果有超时值, 并拷贝离超时时刻还剩的时间到用户空间的timeval中*/

      

       return ret;             // 返回就绪的文件描述符的个数

}

 

==================================================================

       core_sys_select()函数解析

 

int core_sys_select(int n, fd_set __user *inp, fd_set __user *outp,

                        fd_set __user *exp, struct timespec *end_time)

{

       fd_set_bits fds;

       /**

       typedef struct {

              unsigned long *in, *out, *ex;

              unsigned long *res_in, *res_out, *res_ex;

} fd_set_bits;

这个结构体中定义的全是指针，这些指针都是用来指向描述符集合的。

**/

       void *bits;

       int ret, max_fds;

       unsigned int size;

       struct fdtable *fdt;

       /* Allocate small arguments on the stack to save memory and be faster */

       long stack_fds[SELECT_STACK_ALLOC/sizeof(long)];

       // 256/32 = 8, stack中分配的空间

       /**

       @ include/Linux/poll.h

#define FRONTEND_STACK_ALLOC     256

#define SELECT_STACK_ALLOC    FRONTEND_STACK_ALLOC

       **/

 

       ret = -EINVAL;

       if (n < 0)

              goto out_nofds;

 

       /* max_fds can increase, so grab it once to avoid race */

       rcu_read_lock();

       fdt = files_fdtable(current->files); // RCU ref, 获取当前进程的文件描述符表

       max_fds = fdt->max_fds;

       rcu_read_unlock();

       if (n > max_fds)// 如果传入的n大于当前进程最大的文件描述符，给予修正

              n = max_fds;

 

       /*

        * We need 6 bitmaps (in/out/ex for both incoming and outgoing),

        * since we used fdset we need to allocate memory in units of

        * long-words.

        */

       size = FDS_BYTES(n);

       // 以一个文件描述符占一bit来计算，传递进来的这些fd_set需要用掉多少个字

       bits = stack_fds;

       if (size > sizeof(stack_fds) / 6) {

              // 除6，为什么?因为每个文件描述符需要6个bitmaps

              /* Not enough space in on-stack array; must use kmalloc */

              ret = -ENOMEM;

              bits = kmalloc(6 * size, GFP_KERNEL); // stack中分配的太小，直接kmalloc

              if (!bits)

                     goto out_nofds;

       }

       // 这里就可以明显看出struct fd_set_bits结构体的用处了。

       fds.in      = bits;

       fds.out     = bits +   size;

       fds.ex      = bits + 2*size;

       fds.res_in  = bits + 3*size;

       fds.res_out = bits + 4*size;

       fds.res_ex  = bits + 5*size;

       // get_fd_set仅仅调用copy_from_user从用户空间拷贝了fd_set

       if ((ret = get_fd_set(n, inp, fds.in)) ||

           (ret = get_fd_set(n, outp, fds.out)) ||

           (ret = get_fd_set(n, exp, fds.ex)))

              goto out;

       zero_fd_set(n, fds.res_in);  // 对这些存放返回状态的字段清0

       zero_fd_set(n, fds.res_out);

       zero_fd_set(n, fds.res_ex);

 

       ret = do_select(n, &fds, end_time);    // 关键函数，完成主要的工作

 

       if (ret < 0)             // 有错误

              goto out;

       if (!ret) {              // 超时返回，无设备就绪

              ret = -ERESTARTNOHAND;

              if (signal_pending(current))

                     goto out;

              ret = 0;

       }

 

       // 把结果集,拷贝回用户空间

       if (set_fd_set(n, inp, fds.res_in) ||

           set_fd_set(n, outp, fds.res_out) ||

           set_fd_set(n, exp, fds.res_ex))

              ret = -EFAULT;

 

out:

       if (bits != stack_fds)

              kfree(bits);     // 如果有申请空间，那么释放fds对应的空间

out_nofds:

       return ret;                    // 返回就绪的文件描述符的个数

}

 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

do_select()函数解析：

 

int do_select(int n, fd_set_bits *fds, struct timespec *end_time)

{

       ktime_t expire, *to = NULL;

       struct poll_wqueues table;

       poll_table *wait;

       int retval, i, timed_out = 0;

       unsigned long slack = 0;

 

       rcu_read_lock();

       // 根据已经设置好的fd位图检查用户打开的fd, 要求对应fd必须打开, 并且返回

// 最大的fd。

       retval = max_select_fd(n, fds);

       rcu_read_unlock();

 

       if (retval < 0)

              return retval;

       n = retval;

 

       // 一些重要的初始化:

       // poll_wqueues.poll_table.qproc函数指针初始化，该函数是驱动程序中poll函数实

       // 现中必须要调用的poll_wait()中使用的函数。

       poll_initwait(&table);

       wait = &table.pt;

       if (end_time && !end_time->tv_sec && !end_time->tv_nsec) {

              wait = NULL;

              timed_out = 1;     // 如果系统调用带进来的超时时间为0，那么设置

                                          // timed_out = 1，表示不阻塞，直接返回。

       }

 

       if (end_time && !timed_out)

              slack = estimate_accuracy(end_time); // 超时时间转换

 

       retval = 0;

       for (;;) {

              unsigned long *rinp, *routp, *rexp, *inp, *outp, *exp;

 

              inp = fds->in; outp = fds->out; exp = fds->ex;

              rinp = fds->res_in; routp = fds->res_out; rexp = fds->res_ex;

 

              // 所有n个fd的循环

              for (i = 0; i < n; ++rinp, ++routp, ++rexp) {

                     unsigned long in, out, ex, all_bits, bit = 1, mask, j;

                     unsigned long res_in = 0, res_out = 0, res_ex = 0;

                     const struct file_operations *f_op = NULL;

                     struct file *file = NULL;

 

                     // 先取出当前循环周期中的32个文件描述符对应的bitmaps

                     in = *inp++; out = *outp++; ex = *exp++;

                     all_bits = in | out | ex;  // 组合一下，有的fd可能只监测读，或者写，

// 或者e rr，或者同时都监测

                     if (all_bits == 0) {  // 这32个描述符没有任何状态被监测，就跳入

// 下一个32个fd的循环中

                            i += __NFDBITS; //每32个文件描述符一个循环，正好一个long型数

                            continue;

                     }

 

                     // 本次32个fd的循环中有需要监测的状态存在

                     for (j = 0; j < __NFDBITS; ++j, ++i, bit <<= 1) {// 初始bit = 1

                            int fput_needed;

                            if (i >= n)      // i用来检测是否超出了最大待监测的fd

                                   break;

                            if (!(bit & all_bits))

                                   continue; // bit每次循环后左移一位的作用在这里，用来

// 跳过没有状态监测的fd

                            file = fget_light(i, &fput_needed); // 得到file结构指针，并增加

// 引用计数字段f_count

                            if (file) {        // 如果file存在

                                   f_op = file->f_op;

                                   mask = DEFAULT_POLLMASK;

                                   if (f_op && f_op->poll) {

                                          wait_key_set(wait, in, out, bit);// 设置当前fd待监测

//  的事件掩码

                                          mask = (*f_op->poll)(file, wait);

                                          /*

                                                 调用驱动程序中的poll函数，以evdev驱动中的

evdev_poll()为例该函数会调用函数poll_wait(file, &evdev->wait, wait)，继续调用__pollwait()回调来分配一个poll_table_entry结构体，该结构体有一个内嵌的等待队列项，设置好wake时调用的回调函数后将其添加到驱动程序中的等待队列头中。

                                          */

                                   }

                                   fput_light(file, fput_needed);

                                   // 释放file结构指针，实际就是减小他的一个引用

计数字段f_count。

 

                                   // mask是每一个fop->poll()程序返回的设备状态掩码。

                                   if ((mask & POLLIN_SET) && (in & bit)) {

                                          res_in |= bit;         // fd对应的设备可读

                                          retval++;

                                          wait = NULL;       // 后续有用，避免重复执行__pollwait()

                                   }

                                   if ((mask & POLLOUT_SET) && (out & bit)) {

                                          res_out |= bit;              // fd对应的设备可写

                                          retval++;

                                          wait = NULL;

                                   }

                                   if ((mask & POLLEX_SET) && (ex & bit)) {

                                          res_ex |= bit;

                                          retval++;

                                          wait = NULL;

                                   }

                            }

                     }

                     // 根据poll的结果写回到输出位图里,返回给上级函数

                     if (res_in)

                            *rinp = res_in;

                     if (res_out)

                            *routp = res_out;

                     if (res_ex)

                            *rexp = res_ex;

                     /*

                            这里的目的纯粹是为了增加一个抢占点。

                            在支持抢占式调度的内核中（定义了CONFIG_PREEMPT），

cond_resched是空操作。

                     */

                     cond_resched();//内核态的代码在循环体内调用 cond_resched()，yield() 等内核 API，给其它任务得到调度的机会，防止独占滥用 CPU。在内核态写逻辑上造成长时间循环的代码，有可能造成内核死锁或者造成超长调度延迟，尤其是当 Kernel Preemption 没有打开时。这时可以在循环体内调用 cond_resched() 内核 API，有条件的让出 CPU。这里说的有条件是因为cond_resched 要检查 TIF_NEED_RESCHED 标志，看是否有新的 Preemption 的请求。而 yield 内核 API，不检查 TIF_NEED_RESCHED 标志，则无条件触发任务切换，但在所在 CPU Run Queue 没有其它任务的情况下，不会发生真正的任务切换。

              }

              wait = NULL;  // 后续有用，避免重复执行__pollwait()

              if (retval || timed_out || signal_pending(current))

                     break;

              if (table.error) {

                     retval = table.error;

                     break;

              }

              /*跳出这个大循环的条件有: 有设备就绪或有异常(retval!=0), 超时(timed_out

              = 1), 或者有中止信号出现*/

 

              /*

               * If this is the first loop and we have a timeout

               * given, then we convert to ktime_t and set the to

               * pointer to the expiry value.

               */

              if (end_time && !to) {

                     expire = timespec_to_ktime(*end_time);

                     to = &expire;

              }

 

              // 第一次循环中，当前用户进程从这里进入休眠，

// 上面传下来的超时时间只是为了用在睡眠超时这里而已

              // 超时，poll_schedule_timeout()返回0；被唤醒时返回-EINTR

              if (!poll_schedule_timeout(&table, TASK_INTERRUPTIBLE,

                                      to, slack))

                     timed_out = 1; /* 超时后，将其设置成1，方便后面退出循环返回到上层 */

       }

 

       // 清理各个驱动程序的等待队列头，同时释放掉所有空出来

// 的page页(poll_table_entry)

       poll_freewait(&table);

 

       return retval; // 返回就绪的文件描述符的个数

}
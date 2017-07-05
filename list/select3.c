

三、讨论几个细节

      

3.1、fop->poll()

       fop->poll()函数就是file_operations结构体中的poll函数指针项，该函数相信很多人都知道怎么写，网上大把的文章介绍其模板，但是为什么要那么写，而且它做了什么具体的事情？本小节来揭开其神秘面纱，先贴一个模板上来。

       static unsigned int XXX_poll(struct file *filp, poll_table *wait)

{

    unsigned int mask = 0;

        struct XXX_dev *dev = filp->private_data;

    ...

    poll_wait(filp, &dev->r_wait, wait);

    poll_wait(filp ,&dev->w_wait, wait);

   

    if(...)//读就绪

    ｛

          mask |= POLLIN | POLLRDNORM;

     ｝

    if(...)//写就绪

    ｛

          mask |= POLLOUT | POLLRDNORM;

     ｝

    ..

    return mask;

}

       poll_wait()只因有wait字样，经常给人误会，以为它会停在这里等，也就是常说的阻塞。不过我们反过来想想，要是同一个应用进程同时监测多个fd，那么没一个fd调用xxx_poll的时候都阻塞在这里，那和不使用select()又有何区别呢？都会阻塞在当个硬件上而耽误了被的设备就绪事件的读取。

       其实，这个poll_wait()函数所做的工作挺简单，就是添加一个等待等待队列项到poll_wait ()函数传递进去的第二个参数，其代表的是驱动程序中的特定事件的等待队列头。

       下面以字符设备evdev为例，文件drivers/input/evdev.c。

       static unsigned int evdev_poll(struct file *file, poll_table *wait)

{

       struct evdev_client *client = file->private_data;

       struct evdev *evdev = client->evdev;

 

       poll_wait(file, &evdev->wait, wait);

       return ((client->head == client->tail) ? 0 : (POLLIN | POLLRDNORM)) |

              (evdev->exist ? 0 : (POLLHUP | POLLERR));

}

static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address,

poll_table *p)

{

       if (p && wait_address)

              p->qproc(filp, wait_address, p);

}

其中wait_address是驱动程序需要提供的等待队列头，来容纳后续等待该硬件设备就绪的进程对应的等待队列项。关键结构体poll_table, 这个结构体名字也取的不好，什么table？其实其中没有table的一丁点概念，容易让人误解呀！

 

typedef void (*poll_queue_proc)(struct file *, wait_queue_head_t *, struct poll_table_struct *);

typedef struct poll_table_struct {

       poll_queue_proc qproc;

       unsigned long key;

} poll_table;

      fop->poll()函数的poll_table参数是从哪里传进来的？好生阅读过代码就可以发现，do_select()函数中存在一个结构体struct poll_wqueues，其内嵌了一个poll_table的结构体，所以在后面的大循环中依次调用各个fd的fop->poll()传递的poll_table参数都是poll_wqueues.poll_table。

       poll_table结构体的定义其实蛮简单，就一个函数指针，一个key值。这个函数指针在整个select过程中一直不变，而key则会根据不同的fd的监测要求而变化。

      qproc函数初始化在函数do_select()àpoll_initwait()àinit_poll_funcptr(&pwq->pt, __pollwait)中实现，回调函数就是__pollwait()。

       int do_select(int n, fd_set_bits *fds, struct timespec *end_time)

{

       struct poll_wqueues table;

       …

       poll_initwait(&table);

       …

}

void poll_initwait(struct poll_wqueues *pwq)

{

       init_poll_funcptr(&pwq->pt, __pollwait);

       …

}

       static inline void init_poll_funcptr(poll_table *pt, poll_queue_proc qproc)

{

       pt->qproc = qproc;

       pt->key   = ~0UL; /* all events enabled */

}

      

/* Add a new entry */

static void __pollwait(struct file *filp, wait_queue_head_t *wait_address, poll_table *p)

{

       struct poll_wqueues *pwq = container_of(p, struct poll_wqueues, pt);

       struct poll_table_entry *entry = poll_get_entry(pwq);

       if (!entry)

              return;

       get_file(filp);

       entry->filp = filp;         // 保存对应的file结构体

       entry->wait_address = wait_address;  // 保存来自设备驱动程序的等待队列头

       entry->key = p->key;   // 保存对该fd关心的事件掩码

       init_waitqueue_func_entry(&entry->wait, pollwake);

       // 初始化等待队列项，pollwake是唤醒该等待队列项时候调用的函数

       entry->wait.private = pwq;

       // 将poll_wqueues作为该等待队列项的私有数据，后面使用

       add_wait_queue(wait_address, &entry->wait);

       // 将该等待队列项添加到从驱动程序中传递过来的等待队列头中去。

}

该函数首先通过container_of宏来得到结构体poll_wqueues的地址，然后调用poll_get_entry()函数来获得一个poll_table_entry结构体，这个结构体是用来连接驱动和应用进程的关键结构体，其实联系很简单，这个结构体中内嵌了一个等待队列项wait_queue_t，和一个等待队列头 wait_queue_head_t，它就是驱动程序中定义的等待队列头，应用进程就是在这里保存了每一个硬件设备驱动程序中的等待队列头(当然每一个fd都有一个poll_table_entry结构体)。

很容易想到的是，如果这个设备在别的应用程序中也有使用，又恰好别的应用进程中也是用select()来访问该硬件设备，那么在另外一个应用进程的同一个地方也会调用同样的函数来初始化一个poll_table_entry结构体，然后将这个结构体中内嵌的等待队列项添加到同一份驱动程序的等待队列头中。此后，如果设备就绪了，那么驱动程序中将会唤醒这个对于等待队列头中所有的等待队列项(也就是等待在该设备上的所有应用进程，所有等待的应用进程将会得到同一份数据)。

上面红色字体的语句保存了一个应用程序select一个fd的硬件设备时候的最全的信息，方便在设备就绪的时候容易得到对应的数据。这里的entry->key值就是为了防止第二节中描述的误唤醒而准备的。设置这个key值的地方在函数do_select()中。如下：

if (file) {

       f_op = file->f_op;

       mask = DEFAULT_POLLMASK;

       if (f_op && f_op->poll) {

              wait_key_set(wait, in, out, bit);  // 见第二节                                                                  mask = (*f_op->poll)(file, wait);

       }

}

 

fop->poll()函数的返回值都是有规定的，例如函数evdev_poll()中的返回值：

return ((client->head == client->tail) ? 0 : (POLLIN | POLLRDNORM)) |

              (evdev->exist ? 0 : (POLLHUP | POLLERR));

会根据驱动程序中特定的buffer队列标志，来返回设备状态。这里的判断条件是读循环buffer的头尾指针是否相等：client->head == client->tail。

 

       3.2、poll_wait()函数在select()睡眠前后调用的差异

       static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address,

poll_table *p)

{

       if (p && wait_address)

              p->qproc(filp, wait_address, p);

}

这里有一个if条件判断，如果驱动程序中没有提供等待队列头wait_address，那么将不会往下执行p->qproc(__pollwait())，也就是不会将代表当前应用进程的等待队列项添加进驱动程序中对应的等待队列头中。也就是说，如果应用程序恰好用select来监测这个fd的这个等待队列头对应的事件时，是永远也得不到这个设备的就绪或者错误状态的。

       如果select()中调用fop->poll()时传递进来的poll_table是NULL，通常情况下，只要在应用层传递进来的超时时间结构体值不为0，哪怕这个结构体指针你传递NULL，那么在函数do_select()中第一次睡眠之前的那次所有fd的大循环中调用fop->poll()函数传递的poll_table是绝对不会为NULL的，但是第一次睡眠唤醒之后的又一次所有fd的大循环中再次调用fop->poll()函数时，此时传递的poll_table是NULL，可想而知，这一次只是检查fop->poll()的返回状态值而已。最后如果从上层调用select时传递的超时值结构体赋值成0，那么do_select()函数的只会调用一次所有fd的大循环，之后不再进入睡眠，直接返回0给上层，基本上这种情况是没有得到任何有用的状态。

       为了避免应用进程被唤醒之后再次调用pollwait()的时候重复地调用函数__pollwait()，那么在传递poll_table结构体指针的时候，在睡眠之前保证其为有效地址，而在唤醒之后保证传入的poll_table地址是NULL，因为在唤醒之后，再次调用fop->poll()的作用只是为了再次检查设备的事件状态而已。具体详见代码。

      

       3.3、唤醒应用进程

       第二节中已经讨论过驱动程序唤醒进程的一点注意项，但这里再次介绍睡眠唤醒的整个流程。

       睡眠是调用函数poll_schedule_timeout()来实现：

       int poll_schedule_timeout(struct poll_wqueues *pwq, int state,

                       ktime_t *expires, unsigned long slack)

{

       int rc = -EINTR;

 

       set_current_state(state);

       if (!pwq->triggered)  // 这个triggered在什么时候被置1的呢?只要有一个fd

// 对应的设备将当前应用进程唤醒后将会把它设置成1

              rc = schedule_hrtimeout_range(expires, slack, HRTIMER_MODE_ABS);

       __set_current_state(TASK_RUNNING);

      

       set_mb(pwq->triggered, 0);

       return rc;

}

       唤醒的话会调用函数pollwake():

       static int pollwake(wait_queue_t *wait, unsigned mode, int sync, void *key)

{

       struct poll_table_entry *entry;

 

       entry = container_of(wait, struct poll_table_entry, wait);

       if (key && !((unsigned long)key & entry->key))

              return 0;

       return __pollwake(wait, mode, sync, key);

}

       static int __pollwake(wait_queue_t *wait, unsigned mode, int sync, void *key)

{

       struct poll_wqueues *pwq = wait->private;

       DECLARE_WAITQUEUE(dummy_wait, pwq->polling_task);

 

       /*

        * Although this function is called under waitqueue lock, LOCK

        * doesn't imply write barrier and the users expect write

        * barrier semantics on wakeup functions.  The following

        * smp_wmb() is equivalent to smp_wmb() in try_to_wake_up()

        * and is paired with set_mb() in poll_schedule_timeout.

        */

       smp_wmb();

       pwq->triggered = 1;

       // select()用户进程只要有被唤醒过，就不可能再次进入睡眠，因为这个标志在睡眠的时候有用

             

       return default_wake_function(&dummy_wait, mode, sync, key);

       // 默认通用的唤醒函数

}

      

参考网址：

       1. http://blogold.chinaunix.net/u2/60011/showart_1334783.html

http://yuanbor.blog.163.com/blog/static/56674620201051134748647/

http://www.cnblogs.com/hanyan225/archive/2010/10/13/1850497.html

http://hi.baidu.com/operationsystem/blog/item/208eab9821da8f0e6f068cea.html

2. fs/select.c

drivers/input/evdev.c

include/Linux/poll.h

include/linux/wait.h

kernel/wait.c

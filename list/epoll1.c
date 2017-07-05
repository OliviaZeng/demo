第一部分 编程模型
1、服务器编程模型

关于server编程模型，大师stevens在他的《UNP》一书中已经做了详细论述，这里不再重复，这里主要讲一下我的一些理解。

从线程的角度，可以分为两类，一是单线程，一是多线程。先来看单线程模型。
1.1、单线程模型

整个进程只有一个线程，由于只有一个线程，所以要实现高性能，必须与“non-blocking IO + IO multiplexing”相结合。程序基本模型如下：

struct epoll_event ev, events[MAX_EVENTS];

int listen_sock, nfds, epollfd;

 

/* Set up listening socket, 'listen_sock' (socket(),bind(), listen()) */

/* 1. 创建一个epoll描述符 */

epollfd = epoll_create(10);

 

ev.events = EPOLLIN;

ev.data.fd = listen_sock;

 

/* 2. 注册监听事件 */

epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev);

for (;;) {

       /* 3. 监听事件 */

    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

       /* 4. 处理事件 */

    for (n = 0; n < nfds; ++n) {

        if (events[i].data.fd == listener_fd) {

            HandleAccept(events[i].data.fd);

            continue;

        }

 

        if (events[i].events & EPOLLIN) {

            HandleRead(events[i].data.fd);

        }

 

        if (events[i].events & EPOLLOUT) {

            HandleWrite(events[i].data.fd);

        }

    }

}

该模型的代表主要有haproxy、nginx等，另外libevent本身也是单线程的。相对于多线程，单线程server没有线程切换以及加锁的开销，劣势是不能充分利用CPU的多核优势，不过，这可以通过多个进程来解决。

 

另外，这种模型编程也很简单，因为简单，所以是编写高性能server的首选。公司内部的网络框架Srvframework、GNP都属于这类范畴。 Alan Cox曾说：“线程是为不懂状态机的人准备的”[9]。的确，单线程+状态机可以做很多事情。

 
1.2、多线程模型

进程有多个线程，一般来说，可以将server的线程分成两类：I/O线程和工作线程。由此又可以将多线程模型大体分成两类：单一I/O线程+多个工作线程、多个I/O线程(工作线程)。另外，不论是单线程，还是多线程，non-blocking IO + IO multiplexing都是必选的。

 

(1)单一I/O线程+多个工作线程

这种模型下，I/O线程负责event loop、I/O操作，工作线程负责实际的业务逻辑处理，I/O线程与工作线程可以通过阻塞队列/共享内存等方式进行数据交换，队列/共享内存的访问需要加锁。实际上，这种模型本质上与1.2中单线程是类似的，只不过这里的业务逻辑交由单独的工作处理，它有大家都很熟悉的名字——半同步/半异步模型(HS/HA)。Taf属于这类。

 

(2)多个I/O线程(工作线程)

这种模型下，每个I/O线程都有一个event loop，另外，这里的工作线程可有可无，而且通常是没有，即I/O线程既处理I/O，又进行业务逻辑计算。大家熟悉的leader/follower(L/F)以及muti-reactor(M-R)模型都属于这类，这方面的资料很多，见参考文献，不再讨论。Memcached使用的M-R，ICE使用的L/F。

 
1.3、小结

个人认为：(1)单线程模型实现简单，如果业务逻辑不复杂，是实现高性能server的首选，比如proxy之类的server。(2)HS/HA很清晰，如果业务逻辑很复杂，比如database，可以考虑这种模型。(3)如果你想充分利用多CPU，当然可以考虑L/F或者M-R。但是值得一提的是，L/F中会有锁的开销，而M-R中没有锁的开销，但M-R的线程切换的开销要高于L/F。根据同事的一些测试，对于短连接L/F的结果好于M-R，而对于长连接，M-R要好于L/F。

 

第二部分 理解epoll
2、epoll的实现
2.1、核心数据结构
2.1.1、epoll实例

当用系统调用函数epoll_create创建一个epoll环境时，用户态得到epoll fd，内核态创建一个eventpoll：

//fs/eventpoll.c

struct eventpoll {

       /* Protect the access to this structure */

       spinlock_t lock;

 

       /*

        * This mutex is used to ensure that files are not removed

        * while epoll is using them. This is held during the event

        * collection loop, the file cleanup path, the epoll file exit

        * code and the ctl operations.

        */

       struct mutex mtx; ///主要用于epoll_ctl的并发

 

       /* Wait queue used by sys_epoll_wait() */

       wait_queue_head_t wq; ///sys_epoll_wait()使用的等待队列, process list

 

       /* Wait queue used by file->poll() */

       wait_queue_head_t poll_wait; ///file->poll()使用的等待队列

 

       /* List of ready file descriptors */

       struct list_head rdllist; ///ready list

 

       /* RB tree root used to store monitored fd structs */

       struct rb_root rbr;

 

       /*

        * This is a single linked list that chains all the "struct epitem" that

        * happened while transferring ready events to userspace w/out

        * holding ->lock.

        */

       struct epitem *ovflist;

 

       /* The user that created the eventpoll descriptor */

       struct user_struct *user;

}

 

理解这个结构是理解epoll的开始，所以这里有必要解释一下。

我们知道在Unix/Linux中，一切都是文件，对于epoll实例的fd，eventpoll通常保存在file. private_data字段中。

lock：自旋锁，用于保护该数据结构。

mtx：互斥量，主要用于多个epoll_ctl之间的并发，epoll以红黑树组织关注的fd，epoll_ctl会修改红黑树，参见epoll_ctl的实现。

为什么有了一个自旋锁，还要搞一个互斥量？见最后一小结。

wq：epoll_wait使用的等待队列，在多线程程序中，我们可以在多个线程中对同一个epoll实例调用epoll_wait。

poll_wait：这个域是比较让人费解的。这里说说我的理解：对于socket fd，会将fd对应的epitem加入到sock的等待队列，但是，对于epfd，没有sock对象，用poll_wait做等待队列，参见ep_eventpoll_poll。

 

ovflist: 主要是解决当内核在传输数据给用户空间(ep_send_events_proc)时的锁(eventpoll->mtx)，此时epoll就是将这个时候传递上来的事件保存到ovflist中。
2.1.2、epitem

在epoll内部，每个关注的fd都对应一个epitem：

struct epitem {

       /* RB tree node used to link this structure to the eventpoll RB tree */

       struct rb_node rbn; ///RB tree

 

       /* List header used to link this structure to the eventpoll ready list */

       struct list_head rdllink; ///ready list

 

       /*

        * Works together "struct eventpoll"->ovflist in keeping the

        * single linked chain of items.

        */

       struct epitem *next;

 

       /* The file descriptor information this item refers to */

       struct epoll_filefd ffd; ///文件描述符信息

 

       /* Number of active wait queue attached to poll operations */

       int nwait;

 

       /* List containing poll wait queues */

       struct list_head pwqlist;

 

       /* The "container" of this item */

       struct eventpoll *ep;

 

       /* List header used to link this item to the "struct file" items list */

       struct list_head fllink;

 

       /* The structure that describe the interested events and the source fd */

       struct epoll_event event; ///关注的事件

};

这个结构比较简单，没什么好说的。

 
2.2、核心函数
2.2.1、epoll_ctl

当我们调用epoll_create创建一个epoll实例后，就可以调用epoll_ctl向epoll添加关注的fd了。

SYSCALL_DEFINE4(epoll_ctl, int, epfd, int, op, int, fd,

              struct epoll_event __user *, event)

{

…

       ep = file->private_data;

 

    //对互斥量加锁

       mutex_lock(&ep->mtx);

 

       /*

        * Try to lookup the file inside our RB tree, Since we grabbed "mtx"

        * above, we can be sure to be able to use the item looked up by

        * ep_find() till we release the mutex.

        */

       epi = ep_find(ep, tfile, fd);

 

       error = -EINVAL;

       switch (op) {

       case EPOLL_CTL_ADD:

              if (!epi) {

                     epds.events |= POLLERR | POLLHUP;

                     error = ep_insert(ep, &epds, tfile, fd);

              } else

                     error = -EEXIST;

              break;

       case EPOLL_CTL_DEL:

              if (epi)

                     error = ep_remove(ep, epi);

              else

                     error = -ENOENT;

              break;

       case EPOLL_CTL_MOD:

              if (epi) {

                     epds.events |= POLLERR | POLLHUP;

                     error = ep_modify(ep, epi, &epds);

              } else

                     error = -ENOENT;

              break;

       }

       mutex_unlock(&ep->mtx);

…

}

ep_insert：

/* @tfile :target file，即fd对应的file */

static int ep_insert(struct eventpoll *ep, struct epoll_event *event,

                   struct file *tfile, int fd)

{

…

       /* Initialize the poll table using the queue callback */

       epq.epi = epi;

    //设置poll中调用的回调函数ep_ptable_queue_proc

       init_poll_funcptr(&epq.pt, ep_ptable_queue_proc);

 

    /* 调用底层文件系统的poll，对于tcp socket，为sock_poll，后者调用具体协议(protocol-specific)的poll，如tcp_poll( ) */

       revents = tfile->f_op->poll(tfile, &epq.pt); ///参见sock_poll

   

    //加入红黑树

       ep_rbtree_insert(ep, epi);

 

       /* We have to drop the new item inside our item list to keep track of it */

       spin_lock_irqsave(&ep->lock, flags);

 

       /* 如果文件已经ready，则加入到ready list */

       if ((revents & event->events) && !ep_is_linked(&epi->rdllink)) {

              list_add_tail(&epi->rdllink, &ep->rdllist);

 

              /* Notify waiting tasks that events are available */

              if (waitqueue_active(&ep->wq))

                     wake_up_locked(&ep->wq); ///唤醒等待进程,调用epoll_wait的进程

              if (waitqueue_active(&ep->poll_wait))

                     pwake++;

       }

 

       spin_unlock_irqrestore(&ep->lock, flags);

…

}

 ep_insert主要将fd添加到红黑树，然后在具体协议的poll，比如tcp_poll中，调用回调函数ep_ptable_queue_proc，最后检查fd是否ready，如果ready，则将fd加入到就绪队列，并唤醒等待进程。另外，值得一提的是在epoll_ctl和epoll_wait中，对epoll的就绪队列的访问都是由自旋锁lock保护。

/* @file : target file

** @whead: fd对应的sock的等待队列

*/

static void ep_ptable_queue_proc(struct file *file, wait_queue_head_t *whead,

                             poll_table *pt)

{

       struct epitem *epi = ep_item_from_epqueue(pt);

       struct eppoll_entry *pwq;

 

       if (epi->nwait >= 0 && (pwq = kmem_cache_alloc(pwq_cache, GFP_KERNEL))) {

              init_waitqueue_func_entry(&pwq->wait, ep_poll_callback); ///注册唤醒回调函数

              pwq->whead = whead;

              pwq->base = epi;

              add_wait_queue(whead, &pwq->wait); ///将epitem加到sock的等待队列

              list_add_tail(&pwq->llink, &epi->pwqlist);

              epi->nwait++;

       } else {

              /* We have to signal that an error occurred */

              epi->nwait = -1;

       }

}

ep_ptable_queue_proc主要将epoll_ctl传进来的fd封装成的epitem添加到target file对应的sock的等待队列。

 

当socket收到数据时，内核协议栈会将其放到sock的接收队列sk_receive_queue，并调用sk_data_ready回调函数(如果用标准函数sock_init_data来初始化sock实例，通常是sock_def_readable)，唤醒等待队列，内核唤醒原语最终会调用这里注册的回调函数ep_poll_callback。

//net/core/sock.c

static void sock_def_readable(struct sock *sk, int len)

{

       struct socket_wq *wq;

 

       rcu_read_lock();

       wq = rcu_dereference(sk->sk_wq);

       if (wq_has_sleeper(wq)) ///检查sock的等待队列

              wake_up_interruptible_sync_poll(&wq->wait, POLLIN | POLLPRI |

                                          POLLRDNORM | POLLRDBAND); ///唤醒

       sk_wake_async(sk, SOCK_WAKE_WAITD, POLL_IN);

       rcu_read_unlock();

}

下面来看看ep_poll_callback：

static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)

{

…

       struct epitem *epi = ep_item_from_wait(wait);

       struct eventpoll *ep = epi->ep;

 

       spin_lock_irqsave(&ep->lock, flags);

//如果只有ET/ONESHOT位设置,则不加入ready list

       if (!(epi->event.events & ~EP_PRIVATE_BITS))

              goto out_unlock;

    ///check event

       if (key && !((unsigned long) key & epi->event.events))

              goto out_unlock;

 

       /* If this file is already in the ready list we exit soon */

       if (!ep_is_linked(&epi->rdllink)) ///加入ready链表

              list_add_tail(&epi->rdllink, &ep->rdllist);

 

       /*

        * Wake up ( if active ) both the eventpoll wait list and the ->poll()

        * wait list.

        */

       if (waitqueue_active(&ep->wq))

              wake_up_locked(&ep->wq); ///唤醒等待进程

       if (waitqueue_active(&ep->poll_wait))

              pwake++;

out_unlock:

       spin_unlock_irqrestore(&ep->lock, flags);

…

}

该函数将fd加到epoll实例的ready list，然后唤醒epoll实例的等待进程队列。注意红色部分，后面讨论epoll线程安全性的时候会再提到。

 
2.2.2、epoll_wait

当我们把关注的fd添加到epoll环境后，就可以调用epoll_wait等待fd上的事件发生了。

SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,

              int, maxevents, int, timeout)

{

       struct file *file;  ///epfd file

       struct eventpoll *ep;

 

       file = fget(epfd);

 

       ep = file->private_data; ///epoll环境

 

       /* Time to fish for events ... */

       error = ep_poll(ep, events, maxevents, timeout);

…

}

很简单，主要逻辑由ep_poll完成：

static int ep_poll(struct eventpoll *ep, struct epoll_event __user *events,

                 int maxevents, long timeout)

{

…

fetch_events:

       spin_lock_irqsave(&ep->lock, flags);

 

       if (!ep_events_available(ep)) { ///没有ready event, sleep current process

              /*

               * We don't have any available event to return to the caller.

               * We need to sleep here, and we will be wake up by

               * ep_poll_callback() when events will become available.

               */

              init_waitqueue_entry(&wait, current);

///将进程加入到epoll环境的等待队列

              __add_wait_queue_exclusive(&ep->wq, &wait);

 

              for (;;) {

                     /*

                      * We don't want to sleep if the ep_poll_callback() sends us

                      * a wakeup in between. That's why we set the task state

                      * to TASK_INTERRUPTIBLE before doing the checks.

                      */

                     set_current_state(TASK_INTERRUPTIBLE);

                     if (ep_events_available(ep) || timed_out) ///timeout==0, timed_out==1,则break

                            break;

                     if (signal_pending(current)) {

                            res = -EINTR;

                            break;

                     }

 

                     spin_unlock_irqrestore(&ep->lock, flags);

                     if (!schedule_hrtimeout_range(to, slack, HRTIMER_MODE_ABS)) ///schedule

                            timed_out = 1;

 

                     spin_lock_irqsave(&ep->lock, flags);

              }

              __remove_wait_queue(&ep->wq, &wait);

 

              set_current_state(TASK_RUNNING);

       }

check_events:

       /* Is it worth to try to dig for events ? */

       eavail = ep_events_available(ep);

 

       spin_unlock_irqrestore(&ep->lock, flags);

 

       /*

        * Try to transfer events to user space. In case we get 0 events and

        * there's still timeout left over, we go trying again in search of

        * more luck.

        */

       if (!res && eavail && ///将事件拷贝到用户空间

           !(res = ep_send_events(ep, events, maxevents)) && !timed_out)

              goto fetch_events;

…

}

 ep_poll的逻辑也非常简单，就是不断检查epoll的ready list，如果有ready fd，则将其拷贝到用户空间。

注意ep_poll是调用__add_wait_queue_exclusive将当前进程加入到epoll的等待队列的，所以，即使多个线程对同一个epoll调用epoll_wait，也不会出现thundering herd问题。

 

最后来看一下ep_send_events函数，因为它与epoll的几种模式：LT、ET和ONESHOT相关，理解了其实现，也就理解了LT、ET与ONESHOT。

 
2.2.3、ep_send_events

static int ep_send_events(struct eventpoll *ep,

                      struct epoll_event __user *events, int maxevents)

{

       struct ep_send_events_data esed;

 

       esed.maxevents = maxevents;

       esed.events = events;

       ///处理ready list

       return ep_scan_ready_list(ep, ep_send_events_proc, &esed);

}

 

主要逻辑是在回调函数ep_send_events_proc中完成的。

static int ep_send_events_proc(struct eventpoll *ep, struct list_head *head,

                            void *priv)

{

struct ep_send_events_data *esed = priv;

       int eventcnt;

       unsigned int revents;

       struct epitem *epi;

       struct epoll_event __user *uevent;

 

       /*

        * We can loop without lock because we are passed a task private list.

        * Items cannot vanish during the loop because ep_scan_ready_list() is

        * holding "mtx" during this call.

        */

       for (eventcnt = 0, uevent = esed->events;

            !list_empty(head) && eventcnt < esed->maxevents;) {

              epi = list_first_entry(head, struct epitem, rdllink);

 

              list_del_init(&epi->rdllink); ///删除一个epitem

              ///检查事件

              revents = epi->ffd.file->f_op->poll(epi->ffd.file, NULL) &

                     epi->event.events;

 

              /*

               * If the event mask intersect the caller-requested one,

               * deliver the event to userspace. Again, ep_scan_ready_list()

               * is holding "mtx", so no operations coming from userspace

               * can change the item.

               */

              if (revents) {

                     if (__put_user(revents, &uevent->events) ||

                         __put_user(epi->event.data, &uevent->data)) {

                            list_add(&epi->rdllink, head); ///加入到输出队列

                            return eventcnt ? eventcnt : -EFAULT;

                     }

                     eventcnt++;

                     uevent++;

                     if (epi->event.events & EPOLLONESHOT)

///take care,只设置EPOLLET/EPOLLONESHOT,参见ep_poll_callback

                      epi->event.events &= EP_PRIVATE_BITS;

                     else if (!(epi->event.events & EPOLLET)) { //LT模式

                            /*

                             * If this file has been added with Level

                             * Trigger mode, we need to insert back inside

                             * the ready list, so that the next call to

                             * epoll_wait() will check again the events

                             * availability. At this point, no one can insert

                             * into ep->rdllist besides us. The epoll_ctl()

                             * callers are locked out by

                             * ep_scan_ready_list() holding "mtx" and the

                             * poll callback will queue them in ep->ovflist.

                             */

                            list_add_tail(&epi->rdllink, &ep->rdllist); //LT模式,重新加入到ready list

                     }

              }

       }

 

       return eventcnt;

}

LT：Level Triggered，水平触发是epoll的默认工作模式，当fd上有事件发生，内核除了把事件上报给用户，还把fd重新加到就绪队列中，所以直到收集事件时没有事件发生，该fd才从epoll的就绪队列中移除。

         例如：socket fd可读，如果数据并没有读完，则接下来每次epoll_wait都会返回该fd可读，直到有一次收集事件失败，即socket不可读。

 

ET:           Edge Triggered，边缘触发，相比LT，ET收集完事件后不会把fd重新加入就绪队列。

         如果fd可读，epoll上报有事件发生，该fd也从就绪队列中移除了，无论数据有没有读完。该fd只有在下次事件发生并在回调函数ep_poll_callback中被加入就绪队列。

 

ONESHOT: 顾名思义，如果fd有事件发生，只会触发一次。从ep_send_events_pro的实现可以看到，对于EPOLLONESHOT，会将其它事件位全部清掉。这样，以后ep_poll_callback(参见其实现)将不会将该fd加入ready list，即使有事件发生，直到用户再一次通过EPOLL_CTL_MOD重新设置fd。所以，对于ONESHOT的fd，如果有事件发生，每次EPOLL_CTL_MOD只会上报一次。

 

 

第三部分 问题
3、epoll为什么比poll高效

先看看poll的实现：

//fs/select.c

SYSCALL_DEFINE3(poll, struct pollfd __user *, ufds, unsigned int, nfds,

              long, timeout_msecs)

{

…

       ret = do_sys_poll(ufds, nfds, to);

…

}

主要逻辑在do_sys_poll完成：

int do_sys_poll(struct pollfd __user *ufds, unsigned int nfds,

              struct timespec *end_time)

{

       struct poll_wqueues table;

      int err = -EFAULT, fdcount, len, size;

       /* Allocate small arguments on the stack to save memory and be

          faster - use long to make sure the buffer is aligned properly

          on 64 bit archs to avoid unaligned access */

       long stack_pps[POLL_STACK_ALLOC/sizeof(long)];

       struct poll_list *const head = (struct poll_list *)stack_pps; ///先使用栈上的空间

      struct poll_list *walk = head;

      unsigned long todo = nfds;

 

       if (nfds > rlimit(RLIMIT_NOFILE))

              return -EINVAL;

 

       len = min_t(unsigned int, nfds, N_STACK_PPS);

       for (;;) {

              walk->next = NULL;

              walk->len = len;

              if (!len)

                     break;

              ///1.将用户空间的描述符拷贝到内核

              if (copy_from_user(walk->entries, ufds + nfds-todo,

                                   sizeof(struct pollfd) * walk->len))

                     goto out_fds;

 

              todo -= walk->len;

              if (!todo)

                     break;

              ///如果栈上空间不够,则调用kmalloc分配空间存储描述符信息

              len = min(todo, POLLFD_PER_PAGE);

              size = sizeof(struct poll_list) + sizeof(struct pollfd) * len;

              walk = walk->next = kmalloc(size, GFP_KERNEL);

              if (!walk) {

                     err = -ENOMEM;

                     goto out_fds;

              }

       }

       ///设置poll的回调函数,与epoll类似

       poll_initwait(&table);

///2.poll所有描述符,是否有事件发生

       fdcount = do_poll(nfds, head, &table, end_time);

       poll_freewait(&table);

 

       for (walk = head; walk; walk = walk->next) {

              struct pollfd *fds = walk->entries;

              int j;

              ///3.设置相应文件描述发生的事件

              for (j = 0; j < walk->len; j++, ufds++)

                     if (__put_user(fds[j].revents, &ufds->revents))

                            goto out_fds;

    }

…

}

从上面的代码可以看出，poll至少有三个原因导致它比epoll效率低：

(1)每次调用都要将用户空间的所有描述符信息拷贝到内核；

(2)与epoll不同，poll内部没一个ready list，所以，每次都需要检查所有的描述符；

(3)遍历所有的描述符，设置发生的事件。

当fd数量较多时，比如支持上万连接的高并发的server，这些遍历操作会成为性能的致命杀手。
4、epoll线程安全性

考虑两种情况：一是两个线程对同一个epoll调用epoll_ctl；二是A线程对epoll调用epoll_wait，同时线程B调用epoll_ctl。

(1)   从第2节的epoll实现可以看到，epoll_ctl会修改内部红黑树，而且同时涉及到内存分配，所以它们之间通过互斥量保证线程性安全性。

(2)   另外，epoll_wait与epoll_wait，或者epoll_wait与epoll_ctl之间，涉及到对epoll内部ready list的访问，而且时间很短，没有其它复杂逻辑。所以用自旋锁保护。

综上，epoll是线程安全的。

在Memcached中，每个线程都有一个epoll loop，主线程(处理accept)收到一个连接，会丢到工作线程的epoll loop中，它先将连接对象CQ_ITEM加入工作线程的队列，然后通过管道来通知工作线程。但是，由于两个线程需要访问连接对象队列，所以，使用了锁来保护。实际上，如果在主线程中直接对工作线程调用epoll_ctl，是没有问题的，这样，我们就可以做一些工作(比如利用epoll_event数据结构)，从而做到多个线程间无锁编程(这里的无锁是指用户态无锁，但内核仍然会有锁)。注意，这里只针对epoll，不适用于其它模型。

 

主要参考

1. Kernel 3.0 sourcode

2. Libevent 1.4.13 sourcecode

3. Memcached 1.4.5 sourcecode

4. Haproxy 1.4.8 sourcecode

5. Nginx 0.8.28 sourcecode

6. 《Half Sync/Half Async》

7. 《Leader/Followers:A Design Pattern for Efﬁcient Multi-threaded I/O Demultiplexing and Dispatching》

8. 《Understanding Linux Network Internals》

9. http://lkml.indiana.edu/hypermail/linux/kernel/0106.2/0405.html
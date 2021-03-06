前言

在上一篇文章NGINX源码分析：事件模型中，NGINX将epoll封装作为事件模型的核心模块，驱动着NGINX的端口侦听循环和数据收发循环。

epoll是Linux内核为处理大批量文件描述符而作了改进的poll，是Linux下多路复用IO接口select/poll的增强版本，它能显著提高程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率。

简单总结select/poll/epoll三者的区别如下：

    select:

        监听的文件描述符个数有最高上限；

        就绪检查时需要遍历所有事件，效率低；

        内核态和用户态需要内存拷贝，开销大；

    poll：

        没有最大连接数的限制，基于链表来实现；

        就绪检查时需要遍历所有事件，效率低；

        内核态和用户态需要内存拷贝，开销大；

    epoll：

        没有最大连接数的限制，支持的文件描述符上限是最大可以打开文件的数目，1G内存的机器上是大约10万左右；

        就绪检查时只需要遍历已就绪的事件，效率高；

        内核态和用户态不需要内存拷贝，内核可以直接将就绪事件列表添加到用户提供的内存区；

epoll属于已经被写烂了的东西，因为它的实现代码十分精简，只有简单的一个文件”fs/eventpoll.c”，甚至也就刚刚超过2000行，但是它的应用场景却十分广泛，基本上所有高性能Web服务器的事件模型中都封装有epoll的实现。

所以，本文权当备忘记录来简单回顾epoll在Linux内核中的源码实现。
epoll的实现

epoll的系统调用有如下三个：


int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)；
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);

    “epoll_create()”创建一个epoll句柄，同时也占用一个文件描述符；

    “epoll_ctl()”用于向epoll注册一个事件，而且明确监听的事件类型；

    “epoll_wait()”用于检测和等待激活的事件，其中的”events”和”maxevents”提供给内核用于添加就绪事件；

第一部分：epoll的创建

“epoll_create()”用于创建一个目标epoll句柄，显而易见的，epoll需要占用某些系统资源。

Linux内核在系统启动时，其实已经提前完成了该部分资源的分配和初始化。


/* 本文的内核源码版本为linux-3.10.103 */

/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

static int __init eventpoll_init(void)
{
    struct sysinfo si;

    si_meminfo(&si);
    /*
     * Allows top 4% of lomem to be allocated for epoll watches (per user).
     */
    max_user_watches = (((si.totalram - si.totalhigh) / 25) << PAGE_SHIFT) /
        EP_ITEM_COST;
    BUG_ON(max_user_watches < 0);

    /*
     * Initialize the structure used to perform epoll file descriptor
     * inclusion loops checks.
     */
    ep_nested_calls_init(&poll_loop_ncalls);

    /* Initialize the structure used to perform safe poll wait head wake ups */
    ep_nested_calls_init(&poll_safewake_ncalls);

    /* Initialize the structure used to perform file's f_op->poll() calls */
    ep_nested_calls_init(&poll_readywalk_ncalls);

    /*
     * We can have many thousands of epitems, so prevent this from
     * using an extra cache line on 64-bit (and smaller) CPUs
     */
    BUILD_BUG_ON(sizeof(void *) <= 8 && sizeof(struct epitem) > 128);

    /* Allocates slab cache used to allocate "struct epitem" items */
    epi_cache = kmem_cache_create("eventpoll_epi", sizeof(struct epitem),
            0, SLAB_HWCACHE_ALIGN | SLAB_PANIC, NULL);

    /* Allocates slab cache used to allocate "struct eppoll_entry" */
    pwq_cache = kmem_cache_create("eventpoll_pwq",
            sizeof(struct eppoll_entry), 0, SLAB_PANIC, NULL);

    return 0;
}

“eventpoll_init()”在内核模块加载时将被调用来完成epoll相关的资源分配，从其源码可见，Linux内核为epoll提供了”eventpoll_epi”和”eventpoll_pwq”两块基于slab机制的内存单元，我们可以通过”proc”文件系统直接观察到该块内存，这里需要自行比对内核版本观察..。


shell# cat /proc/slabinfo

很明显，epoll中对象的分配和释放操作非常频繁，slab可以最大程度提升epoll使用内存的效率。

接着再来分析”epoll_create()”的源码：


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * Open an eventpoll file descriptor.
 */
SYSCALL_DEFINE1(epoll_create1, int, flags)
{
    int error, fd;
    struct eventpoll *ep = NULL;
    struct file *file;

    /* Check the EPOLL_* constant for consistency.  */
    BUILD_BUG_ON(EPOLL_CLOEXEC != O_CLOEXEC);

    if (flags & ~EPOLL_CLOEXEC)
        return -EINVAL;
    /*
     * Create the internal data structure ("struct eventpoll").
     */
    error = ep_alloc(&ep);
    if (error < 0)
        return error;
    /*
     * Creates all the items needed to setup an eventpoll file. That is,
     * a file structure and a free file descriptor.
     */
    fd = get_unused_fd_flags(O_RDWR | (flags & O_CLOEXEC));
    if (fd < 0) {
        error = fd;
        goto out_free_ep;
    }
    file = anon_inode_getfile("[eventpoll]", &eventpoll_fops, ep,
                 O_RDWR | (flags & O_CLOEXEC));
    if (IS_ERR(file)) {
        error = PTR_ERR(file);
        goto out_free_fd;
    }
    ep->file = file;
    fd_install(fd, file);
    return fd;

out_free_fd:
    put_unused_fd(fd);
out_free_ep:
    ep_free(ep);
    return error;
}

可以看到，”epoll_create()”的逻辑也不复杂，”ep_alloc()”为epoll创建了一个”struct eventpoll”对象，这是epoll的核心结构，它通过”fd”和”file”关联起来，后续所有的针对epoll的操作直接通过这个相关联的”fd”来操作即可。

“fd”用来操作epoll，”file”用来关联Linux中的某个文件，因为epoll是作为文件形式存在的，但其实是以匿名的方式，因为它只要可以和对应的inode关联起来即可，不需要更复杂的文件操作。

“anon_inode_getfile()”完成匿名文件的创建，还有很重要的一点，epoll通过该函数将”ep”和”file->private_data”关联起来。

另外，”ep_alloc()”在实例化epoll对象的时候，会初始化一系列非常重要的成员。


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

static int ep_alloc(struct eventpoll **pep)
{
    int error;
    struct user_struct *user;
    struct eventpoll *ep;

    user = get_current_user();
    error = -ENOMEM;
    ep = kzalloc(sizeof(*ep), GFP_KERNEL);
    if (unlikely(!ep))
        goto free_uid;

    spin_lock_init(&ep->lock);
    mutex_init(&ep->mtx);
    init_waitqueue_head(&ep->wq);
    init_waitqueue_head(&ep->poll_wait);
    INIT_LIST_HEAD(&ep->rdllist);
    ep->rbr = RB_ROOT;
    ep->ovflist = EP_UNACTIVE_PTR;
    ep->user = user;

    *pep = ep;

    return 0;

free_uid:
    free_uid(user);
    return error;
}

    ep->wq: “wq”是”epoll_wait()”在等待事件就绪时将会用到的等待队列；

    ep->poll_wait: “poll_wait”是

    ep->rdllist：”rdllist”是用来保存epoll就绪事件的就绪队列;

第二部分：epoll的事件注册

“epoll_ctl()”用于向epoll注册一个事件，”epfd”指定”epoll_create()”返回的”fd”，op”指定事件增删改的操作，”fd”指定需要监听的套接字或者其它”fd”，”event”指定监听事件方式(如LT/ET..)，其源码如下：


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * The following function implements the controller interface for
 * the eventpoll file that enables the insertion/removal/change of
 * file descriptors inside the interest set.
 */
SYSCALL_DEFINE4(epoll_ctl, int, epfd, int, op, int, fd,
        struct epoll_event __user *, event)
{
    int error;
    int did_lock_epmutex = 0;
    struct file *file, *tfile;
    struct eventpoll *ep;
    struct epitem *epi;
    struct epoll_event epds;

    error = -EFAULT;
    if (ep_op_has_event(op) &&
        copy_from_user(&epds, event, sizeof(struct epoll_event)))
        goto error_return;

    /* Get the "struct file *" for the eventpoll file */
    error = -EBADF;
    file = fget(epfd);
    if (!file)
        goto error_return;

    /* Get the "struct file *" for the target file */
    tfile = fget(fd);
    if (!tfile)
        goto error_fput;

    /* The target file descriptor must support poll */
    error = -EPERM;
    if (!tfile->f_op || !tfile->f_op->poll)
        goto error_tgt_fput;

    /* Check if EPOLLWAKEUP is allowed */
    if ((epds.events & EPOLLWAKEUP) && !capable(CAP_BLOCK_SUSPEND))
        epds.events &= ~EPOLLWAKEUP;

    /*
     * We have to check that the file structure underneath the file descriptor
     * the user passed to us _is_ an eventpoll file. And also we do not permit
     * adding an epoll file descriptor inside itself.
     */
    error = -EINVAL;

    /* epoll_ctl有两个file，file是epoll的文件描述符，tfile是监听的文件描述符 */

    if (file == tfile || !is_file_epoll(file))
        goto error_tgt_fput;

    /*
     * At this point it is safe to assume that the "private_data" contains
     * our own data structure.
     */
    ep = file->private_data;

    /*
     * When we insert an epoll file descriptor, inside another epoll file
     * descriptor, there is the change of creating closed loops, which are
     * better be handled here, than in more critical paths. While we are
     * checking for loops we also determine the list of files reachable
     * and hang them on the tfile_check_list, so we can check that we
     * haven't created too many possible wakeup paths.
     *
     * We need to hold the epmutex across both ep_insert and ep_remove
     * b/c we want to make sure we are looking at a coherent view of
     * epoll network.
     */

    /* 这里的一大串注释很清晰解释了epoll对于循环监听的异常处理 */

    if (op == EPOLL_CTL_ADD || op == EPOLL_CTL_DEL) {
        mutex_lock(&epmutex);
        did_lock_epmutex = 1;
    }
    if (op == EPOLL_CTL_ADD) {
        if (is_file_epoll(tfile)) {
            error = -ELOOP;
            if (ep_loop_check(ep, tfile) != 0) {
                clear_tfile_check_list();
                goto error_tgt_fput;
            }
        } else
            list_add(&tfile->f_tfile_llink, &tfile_check_list);
    }

    mutex_lock_nested(&ep->mtx, 0);

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
        clear_tfile_check_list();
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

error_tgt_fput:
    if (did_lock_epmutex)
        mutex_unlock(&epmutex);

    fput(tfile);
error_fput:
    fput(file);
error_return:

    return error;
}

“ep_find()”之前的代码用来做一些异常处理操作，epoll是不允许循环监听的，即”epoll_ctl()”监听的”fd”不能是epoll文件描述符本身。

“epoll_ctl()”的核心代码在”ep_find()”之后，其实就是”switch”语句里的EPOLL_CTL_ADD/EPOLL_CTL_DEL/EPOLL_CTL_MOD”来完成事件的注册、删除和修改，后两者的代码不是很复杂，我们直接来关注核心的事件注册函数”ep_insert()”。


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * Must be called with "mtx" held.
 */
static int ep_insert(struct eventpoll *ep, struct epoll_event *event,
             struct file *tfile, int fd)
{
    int error, revents, pwake = 0;
    unsigned long flags;
    long user_watches;
    struct epitem *epi;
    struct ep_pqueue epq;

    user_watches = atomic_long_read(&ep->user->epoll_watches);
    if (unlikely(user_watches >= max_user_watches))
        return -ENOSPC;

    /* slab可以提高epitem的分配和回收效率 */

    if (!(epi = kmem_cache_alloc(epi_cache, GFP_KERNEL)))
        return -ENOMEM;

    /* 三个队列的初始化，就绪队列、关联文件队列、等待队列 */   

    /* Item initialization follow here ... */
    INIT_LIST_HEAD(&epi->rdllink);
    INIT_LIST_HEAD(&epi->fllink);
    INIT_LIST_HEAD(&epi->pwqlist);

    /* 将epitem和对应的epoll关联起来 */

    epi->ep = ep;
    ep_set_ffd(&epi->ffd, tfile, fd);
    epi->event = *event;
    epi->nwait = 0;
    epi->next = EP_UNACTIVE_PTR;

    /* epoll的自动休眠问题，有兴趣可以阅读EPOLLWAKEUP的注释 */

    if (epi->event.events & EPOLLWAKEUP) {
        error = ep_create_wakeup_source(epi);
        if (error)
            goto error_create_wakeup_source;
    } else {
        RCU_INIT_POINTER(epi->ws, NULL);
    }

    /* 设置epoll的poll table，其实就是设置epq.pt的回调函数为ep_ptable_queue_proc */

    /* Initialize the poll table using the queue callback */
    epq.epi = epi;
    init_poll_funcptr(&epq.pt, ep_ptable_queue_proc);

    /*
     * Attach the item to the poll hooks and get current event bits.
     * We can safely use the file* here because its usage count has
     * been increased by the caller of this function. Note that after
     * this operation completes, the poll callback can start hitting
     * the new item.
     */

    /* 将上面设置好回调函数的epq.pt传入驱动层 */
    revents = ep_item_poll(epi, &epq.pt);

    /*
     * We have to check if something went wrong during the poll wait queue
     * install process. Namely an allocation for a wait queue failed due
     * high memory pressure.
     */
    error = -ENOMEM;
    if (epi->nwait < 0)
        goto error_unregister;

    /* Add the current item to the list of active epoll hook for this file */
    spin_lock(&tfile->f_lock);
    list_add_tail(&epi->fllink, &tfile->f_ep_links);
    spin_unlock(&tfile->f_lock);

    /* 添加epitem至epoll的红黑树中 */

    /*
     * Add the current item to the RB tree. All RB tree operations are
     * protected by "mtx", and ep_insert() is called with "mtx" held.
     */
    ep_rbtree_insert(ep, epi);

    /* now check if we've created too many backpaths */
    error = -EINVAL;
    if (reverse_path_check())
        goto error_remove_epi;

    /* We have to drop the new item inside our item list to keep track of it */
    spin_lock_irqsave(&ep->lock, flags);

    /* If the file is already "ready" we drop it inside the ready list */
    if ((revents & event->events) && !ep_is_linked(&epi->rdllink)) {
        list_add_tail(&epi->rdllink, &ep->rdllist);
        ep_pm_stay_awake(epi);

        /* Notify waiting tasks that events are available */
        if (waitqueue_active(&ep->wq))
            wake_up_locked(&ep->wq);
        if (waitqueue_active(&ep->poll_wait))
            pwake++;
    }

    spin_unlock_irqrestore(&ep->lock, flags);

    atomic_long_inc(&ep->user->epoll_watches);

    /* We have to call this outside the lock */
    if (pwake)
        ep_poll_safewake(&ep->poll_wait);

    return 0;

error_remove_epi:
    spin_lock(&tfile->f_lock);
    if (ep_is_linked(&epi->fllink))
        list_del_init(&epi->fllink);
    spin_unlock(&tfile->f_lock);

    rb_erase(&epi->rbn, &ep->rbr);

error_unregister:
    ep_unregister_pollwait(ep, epi);

    /*
     * We need to do this because an event could have been arrived on some
     * allocated wait queue. Note that we don't care about the ep->ovflist
     * list, since that is used/cleaned only inside a section bound by "mtx".
     * And ep_insert() is called with "mtx" held.
     */
    spin_lock_irqsave(&ep->lock, flags);
    if (ep_is_linked(&epi->rdllink))
        list_del_init(&epi->rdllink);
    spin_unlock_irqrestore(&ep->lock, flags);

    wakeup_source_unregister(ep_wakeup_source(epi));

error_create_wakeup_source:
    kmem_cache_free(epi_cache, epi);

    return error;
}

INIT_LIST_HEAD完成epoll事件对象”epitem”相关的三个队列的初始化：

    rdllink: epitem的事件就绪队列；

    fllink: epitem的关联文件队列；

    pwqlist: epitem的等待队列；

这三个队列非常重要，”fllink”用来记录epitem关联的文件句柄（释放资源时会用到），”rdllink”和”pwqlist”后面会继续分析。

epoll在注册事件的时候会设置其回调函数为”ep_ptable_queue_proc()”，当有事件到达时，驱动层会调用该回调函数完成事件的激活。

“ep_item_poll()”会调用对应的”fd”的poll操作, 看是否有事件到达，有兴趣的可以去阅读驱动层的”poll_wait()”，这部分有机会再讲。

epoll在注册事件时会将事件对象插入epoll的红黑树”rbr”中，时间开销为O(logn)，n为事件的总数，一般情况下都是总连接数。

“ep_insert()”也同时会检测当前”fd”代表的文件是否已经就绪（红黑树后的代码），若已就绪，则直接将其添加至”rdllink”，然后唤醒等待中的目标进程；
第三部分：epoll的事件就绪

在epoll的事件注册中提到的”ep_ptable_queue_proc()”将epoll的事件对象和设备（主要是网卡）驱动程序关联起来，当相应的事件发生后，驱动层就会调用该回调函数。


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * This is the callback that is used to add our wait queue to the
 * target file wakeup lists.
 */
static void ep_ptable_queue_proc(struct file *file, wait_queue_head_t *whead,
                 poll_table *pt)
{
    struct epitem *epi = ep_item_from_epqueue(pt);
    struct eppoll_entry *pwq;

    if (epi->nwait >= 0 && (pwq = kmem_cache_alloc(pwq_cache, GFP_KERNEL))) {
        init_waitqueue_func_entry(&pwq->wait, ep_poll_callback);
        pwq->whead = whead;
        pwq->base = epi;
        add_wait_queue(whead, &pwq->wait);
        list_add_tail(&pwq->llink, &epi->pwqlist);
        epi->nwait++;
    } else {
        /* We have to signal that an error occurred */
        epi->nwait = -1;
    }
}

可以看到，该回调函数的核心逻辑就是利用Linux内核的等待队列，将”eppoll_entry”对象添加到epitem的等待队列”pwqlist”中，同时设置其回调函数为”ep_poll_callback()”。

“ep_ptable_queue_proc()”需要配合驱动层的代码来完成回调，主要是TCP/IP协议栈的实现代码，我们这里不展开，默认当数据到达驱动层时，”ep_poll_callback()”将被回调。


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * This is the callback that is passed to the wait queue wakeup
 * mechanism. It is called by the stored file descriptors when they
 * have events to report.
 */
static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
    int pwake = 0;
    unsigned long flags;
    struct epitem *epi = ep_item_from_wait(wait);
    struct eventpoll *ep = epi->ep;

    if ((unsigned long)key & POLLFREE) {
        ep_pwq_from_wait(wait)->whead = NULL;
        /*
         * whead = NULL above can race with ep_remove_wait_queue()
         * which can do another remove_wait_queue() after us, so we
         * can't use __remove_wait_queue(). whead->lock is held by
         * the caller.
         */
        list_del_init(&wait->task_list);
    }

    spin_lock_irqsave(&ep->lock, flags);

    /*
     * If the event mask does not contain any poll(2) event, we consider the
     * descriptor to be disabled. This condition is likely the effect of the
     * EPOLLONESHOT bit that disables the descriptor when an event is received,
     * until the next EPOLL_CTL_MOD will be issued.
     */
    if (!(epi->event.events & ~EP_PRIVATE_BITS))
        goto out_unlock;

    /*
     * Check the events coming with the callback. At this stage, not
     * every device reports the events in the "key" parameter of the
     * callback. We need to be able to handle both cases here, hence the
     * test for "key" != NULL before the event match test.
     */
    if (key && !((unsigned long) key & epi->event.events))
        goto out_unlock;

    /*
     * If we are transferring events to userspace, we can hold no locks
     * (because we're accessing user memory, and because of linux f_op->poll()
     * semantics). All the events that happen during that period of time are
     * chained in ep->ovflist and requeued later on.
     */
    if (unlikely(ep->ovflist != EP_UNACTIVE_PTR)) {
        if (epi->next == EP_UNACTIVE_PTR) {
            epi->next = ep->ovflist;
            ep->ovflist = epi;
            if (epi->ws) {
                /*
                 * Activate ep->ws since epi->ws may get
                 * deactivated at any time.
                 */
                __pm_stay_awake(ep->ws);
            }

        }
        goto out_unlock;
    }

    /* 添加就绪事件至rdllist */

    /* If this file is already in the ready list we exit soon */
    if (!ep_is_linked(&epi->rdllink)) {
        list_add_tail(&epi->rdllink, &ep->rdllist);
        ep_pm_stay_awake_rcu(epi);
    }

    /*
     * Wake up ( if active ) both the eventpoll wait list and the ->poll()
     * wait list.
     */

    /* 唤醒阻塞在epoll_wait()的进程 */

    if (waitqueue_active(&ep->wq))
        wake_up_locked(&ep->wq);

    /* 唤醒阻塞在eventpoll文件的进程 */

    if (waitqueue_active(&ep->poll_wait))
        pwake++;

out_unlock:
    spin_unlock_irqrestore(&ep->lock, flags);

    /* We have to call this outside the lock */
    if (pwake)
        ep_poll_safewake(&ep->poll_wait);

    return 1;
}

“ep_poll_callback()”的作用主要有有两个：

    添加就绪事件至”rdllist”；

    唤醒阻塞在”epoll_wait()”的进程；

我们最后再来分析”epoll_wait()”的源码：


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/*
 * Implement the event wait interface for the eventpoll file. It is the kernel
 * part of the user space epoll_wait(2).
 */
SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,
        int, maxevents, int, timeout)
{
    int error;
    struct fd f;
    struct eventpoll *ep;

    /* The maximum number of event must be greater than zero */
    if (maxevents <= 0 || maxevents > EP_MAX_EVENTS)
        return -EINVAL;

    /* Verify that the area passed by the user is writeable */
    if (!access_ok(VERIFY_WRITE, events, maxevents * sizeof(struct epoll_event)))
        return -EFAULT;

    /* Get the "struct file *" for the eventpoll file */
    f = fdget(epfd);
    if (!f.file)
        return -EBADF;

    /*
     * We have to check that the file structure underneath the fd
     * the user passed to us _is_ an eventpoll file.
     */
    error = -EINVAL;
    if (!is_file_epoll(f.file))
        goto error_fput;

    /*
     * At this point it is safe to assume that the "private_data" contains
     * our own data structure.
     */
    ep = f.file->private_data;

    /* Time to fish for events ... */
    error = ep_poll(ep, events, maxevents, timeout);

error_fput:
    fdput(f);
    return error;
}

这部分代码很简略，我们主要关注的对象是入参的”events”和”maxevents”和函数”ep_poll()”，”events”和”maxevents”是用户提供给内核用于添加就绪事件的地方，这样可以避免用户空间和内核空间的多次拷贝。

“ep_poll()”的代码如下：


/* linux-3.10.103/linux-3.10.103/fs/eventpoll.c */

/**
 * ep_poll - Retrieves ready events, and delivers them to the caller supplied
 *           event buffer.
 *
 * @ep: Pointer to the eventpoll context.
 * @events: Pointer to the userspace buffer where the ready events should be
 *          stored.
 * @maxevents: Size (in terms of number of events) of the caller event buffer.
 * @timeout: Maximum timeout for the ready events fetch operation, in
 *           milliseconds. If the @timeout is zero, the function will not block,
 *           while if the @timeout is less than zero, the function will block
 *           until at least one event has been retrieved (or an error
 *           occurred).
 *
 * Returns: Returns the number of ready events which have been fetched, or an
 *          error code, in case of error.
 */
static int ep_poll(struct eventpoll *ep, struct epoll_event __user *events,
           int maxevents, long timeout)
{
    int res = 0, eavail, timed_out = 0;
    unsigned long flags;
    long slack = 0;
    wait_queue_t wait;
    ktime_t expires, *to = NULL;

    if (timeout > 0) {
        struct timespec end_time = ep_set_mstimeout(timeout);

        slack = select_estimate_accuracy(&end_time);
        to = &expires;
        *to = timespec_to_ktime(end_time);
    } else if (timeout == 0) {
        /*
         * Avoid the unnecessary trip to the wait queue loop, if the
         * caller specified a non blocking operation.
         */
        timed_out = 1;
        spin_lock_irqsave(&ep->lock, flags);
        goto check_events;
    }

fetch_events:
    spin_lock_irqsave(&ep->lock, flags);

    /* 如果没有就绪事件，则持续等待，直到有事件被激活或者超时 */

    if (!ep_events_available(ep)) {
        /*
         * We don't have any available event to return to the caller.
         * We need to sleep here, and we will be wake up by
         * ep_poll_callback() when events will become available.
         */

        /* 进程将在此休眠，直到上文中的"ep_poll_callback()"在有事件就绪时唤醒该进程 */

        init_waitqueue_entry(&wait, current);
        __add_wait_queue_exclusive(&ep->wq, &wait);

        for (;;) {
            /*
             * We don't want to sleep if the ep_poll_callback() sends us
             * a wakeup in between. That's why we set the task state
             * to TASK_INTERRUPTIBLE before doing the checks.
             */
            set_current_state(TASK_INTERRUPTIBLE);
            if (ep_events_available(ep) || timed_out)
                break;
            if (signal_pending(current)) {
                res = -EINTR;
                break;
            }

            spin_unlock_irqrestore(&ep->lock, flags);
            if (!schedule_hrtimeout_range(to, slack, HRTIMER_MODE_ABS))
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

    /* 注释很明白地解释了内核在此将就绪事件拷贝至用户空间中 */

    if (!res && eavail &&
        !(res = ep_send_events(ep, events, maxevents)) && !timed_out)
        goto fetch_events;

    return res;
}

epoll的效率比select和poll更高的一个重要原因就在于就绪检查时只需要遍历已就绪的事件，通过上述分析，也验证了其原理。

epoll在实现”epoll_wait()”的时候，不需要去遍历所有的事件, 它通过回调机制把就绪事件主动拷贝到就绪队列”rdllist”中。

另外，epoll有效地利用Linux内核的等待队列，可以在有事件就绪时可以及时唤醒阻塞在”epoll_wait()”上的进程。
总结

epoll的代码确实比较简洁紧凑，但是其实现机制主要还是依赖于Linux内核的等待队列，所以其回调模型也不容易理解。

通过分析epoll的源码实现，也可以很容易明白其效率对比select和poll更高效的原因和实现原理。

但是epoll其实也存在一些问题，epoll对于事件的添加和删除依赖红黑树的特性，红黑树在添加和删除结点时其时间复杂度为O(logn)，因此在大连接量下频繁建立和关闭连接时仍然有一定的性能问题（超短连接场景下），所以，epoll的适用场景主要还是在于活跃连接数有限时。
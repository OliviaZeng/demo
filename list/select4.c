	int do_select(int n, fd_set_bits *fds, struct timespec *end_time)
{
	struct poll_wqueues table;
	…

////初始化
	poll_initwait(&table);//主要初始了qproc这个函数指针，
   …
}
void poll_initwait(struct poll_wqueues *pwq)
{
	init_poll_funcptr(&pwq->pt, __pollwait); //__pollwait从哪里来的？？
	…
}
	static inline void init_poll_funcptr(poll_table *pt, poll_queue_proc qproc)
{
	pt->qproc = qproc;
	pt->key   = ~0UL; /* all events enabled */
}
	


//每个fd调用对应驱动的poll函数时，1、分配poll_table_entry以便建立驱动和应用进程的连接；2、将要检测的等待队列项(进程)加到等待队列头中；3、注册唤醒回调函数pollwake()

对每一个fd调用fop->poll()->poll_wait()->__pollwait()
fop->poll()模板：
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

具体例子：
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



/* Add a new entry */
static void __pollwait(struct file *filp, wait_queue_head_t *wait_address, poll_table *p)
{
	struct poll_wqueues *pwq = container_of(p, struct poll_wqueues, pt);
	struct poll_table_entry *entry = poll_get_entry(pwq);
	if (!entry)
		return;
	get_file(filp);
	entry->filp = filp;		// 保存对应的file结构体
	entry->wait_address = wait_address;	// 保存来自设备驱动程序的等待队列头
	entry->key = p->key; 	// 保存对该fd关心的事件掩码
	init_waitqueue_func_entry(&entry->wait, pollwake);
	// 初始化等待队列项，pollwake是唤醒该等待队列项时候调用的函数
	entry->wait.private = pwq;
	// 将poll_wqueues作为该等待队列项的私有数据，后面使用
	add_wait_queue(wait_address, &entry->wait); 
	// 将该等待队列项添加到从驱动程序中传递过来的等待队列头中去。
}
static int pollwake(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
	struct poll_table_entry *entry;
 
	entry = container_of(wait, struct poll_table_entry, wait);
	// 取得poll_table_entry结构体指针
	if (key && !((unsigned long)key & entry->key))
	/*这里的条件判断至关重要，避免应用进程被误唤醒，什么意思？*/
		return 0;
	return __pollwake(wait, mode, sync, key);
}




typedef struct poll_table_struct {
	poll_queue_proc qproc;
	unsigned long key;
} poll_table;

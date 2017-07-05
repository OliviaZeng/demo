二、重要结构体之间关系

       比较重要的结构体由四个：struct poll_wqueues、struct poll_table_page、struct poll_table_entry、struct poll_table_struct，这小节重点讨论前三个，后面一个留到后面小节。

      

2.1、结构体关系

每一个调用select()系统调用的应用进程都会存在一个struct poll_weueues结构体，用来统一辅佐实现这个进程中所有待监测的fd的轮询工作，后面所有的工作和都这个结构体有关，所以它非常重要。

struct poll_wqueues {

       poll_table pt;

       struct poll_table_page *table;

       struct task_struct *polling_task; //保存当前调用select的用户进程struct task_struct结构体

       int triggered;         // 当前用户进程被唤醒后置成1，以免该进程接着进睡眠

       int error;               // 错误码

       int inline_index;   // 数组inline_entries的引用下标

       struct poll_table_entry inline_entries[N_INLINE_POLL_ENTRIES];

};

实际上结构体poll_wqueues内嵌的poll_table_entry数组inline_entries[] 的大小是有限：

#define MAX_STACK_ALLOC 832

#define FRONTEND_STACK_ALLOC     256

#define WQUEUES_STACK_ALLOC     

(MAX_STACK_ALLOC - FRONTEND_STACK_ALLOC)

#define N_INLINE_POLL_ENTRIES      

(WQUEUES_STACK_ALLOC / sizeof(struct poll_table_entry))

如果空间不够用，后续会动态申请物理内存页以链表的形式挂载poll_wqueues.table上统一管理。接下来的两个结构体就和这项内容密切相关：

struct poll_table_page { // 申请的物理页都会将起始地址强制转换成该结构体指针

       struct poll_table_page * next;     // 指向下一个申请的物理页

       struct poll_table_entry * entry; // 指向entries[]中首个待分配(空的) poll_table_entry地址

       struct poll_table_entry entries[0]; // 该page页后面剩余的空间都是待分配的

//  poll_table_entry结构体

};

对每一个fd调用fop->poll()  à  poll_wait()  à  __pollwait()都会先从poll_wqueues. inline_entries[]中分配一个poll_table_entry结构体，直到该数组用完才会分配物理页挂在链表指针poll_wqueues.table上然后才会分配一个poll_table_entry结构体。具体用来做什么？这里先简单说说，__pollwait()函数调用时需要3个参数，第一个是特定fd对应的file结构体指针，第二个就是特定fd对应的硬件驱动程序中的等待队列头指针，第3个是调用select()的应用进程中poll_wqueues结构体的poll_table项(该进程监测的所有fd调用fop->poll函数都用这一个poll_table结构体)。

struct poll_table_entry {

       struct file *filp;            // 指向特定fd对应的file结构体;

       unsigned long key;              // 等待特定fd对应硬件设备的事件掩码，如POLLIN、

//  POLLOUT、POLLERR;

       wait_queue_t wait;             // 代表调用select()的应用进程，等待在fd对应设备的特定事件

//  (读或者写)的等待队列头上，的等待队列项;

       wait_queue_head_t *wait_address; // 设备驱动程序中特定事件的等待队列头；

};

       总结一下几点：

1.               特定的硬件设备驱动程序的事件等待队列头是有限个数的，通常是有读事件和写事件的等待队列头；

2.               而一个调用了select()的应用进程只存在一个poll_wqueues结构体；

3.               该应用程序可以有多个fd在进行同时监测其各自的事件发生，但该应用进程中每一个fd有多少个poll_table_entry存在，那就取决于fd对应的驱动程序中有几个事件等待队列头了，也就是说，通常驱动程序的poll函数中需要对每一个事件的等待队列头调用poll_wait()函数。比如，如果有读写两个等待队列头，那么就在这个应用进程中存在两个poll_table_entry结构体，在这两个事件的等待队列头中分别将两个等待队列项加入；

4.               如果有多个应用进程使用selcet()方式同时在访问同一个硬件设备，此时硬件驱动程序中加入等待队列头中的等待队列项对每一个应用程序来说都是相同数量的(一个事件等待队列头一个，数量取决于事件等待队列头的个数)。

 

2.2、注意项

对于第3点中，如果驱动程序中有多个事件等待队列头，那么在这种情况下，写设备驱动程序时就要特别小心了，特别是设备有事件就绪然后唤醒等待队列头中所有应用进程的时候需要使用另外的宏，唤醒使用的宏和函数源码见include/Linux/wait.h:

在这之前看一看__pollwait()函数中填充poll_table_entry结构体时注册的唤醒回调函数pollwake()。

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

到底什么情况下会出现误唤醒呢？当然是有先决条件的。

驱动程序中存在多个事件的等待队列头，并且应用程序中只监测了该硬件的某几项事件，比如，驱动中有读写等待队里头，但应用程序中只有在监测读事件的发生。这种情况下，写驱动程序时候，如果唤醒函数用法不当，就会引起误唤醒的情况。

先来看一看我们熟知的一些唤醒函数吧！

#define wake_up(x)                    __wake_up(x, TASK_NORMAL, 1, NULL)

#define wake_up_interruptible(x)      __wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)

void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr, void *key);

注意到这个key了吗？通常我们调用唤醒函数时key为NULL，很容易看出，如果我们在这种情况下，使用上面两种唤醒函数，那么上面红色字体的判断条件一直都会是假，那么也就是说，只要设备的几类事件之一有发生，不管应用程序中是否对其有监测，都会在这里顺利通过将应用程序唤醒，唤醒后，重新调用一遍fop->poll(注意：第一次和第二次调用该函数时少做了一件事，后面代码详解)函数，得到设备事件掩码。假如恰好在这次唤醒后的一轮调用fop->poll()函数的循环中，没有其他硬件设备就绪，那么可想而知，从源码上看，do_select()会直接返回0。

// mask是每一个fop->poll()程序返回的设备状态掩码。

if ((mask & POLLIN_SET) && (in & bit)) {

       res_in |= bit;         // fd对应的设备可读

       retval++;

       wait = NULL;              // 后续有用，避免重复执行__pollwait()

}

(in & bit)这个条件就是用来确认用户程序有没有让你监测该事件的, 如果没有retval仍然是0，基于前面的假设，那么do_select()返回给上层的也是0。那又假如应用程序中调用select()的时候没有传入超时值，那岂不是和事实不相符合吗？没有传递超时值，那么select()函数会一直阻塞直到至少有1个fd的状态就绪。

所以在这种情况下，设备驱动中唤醒函数需要用另外的一组：

#define wake_up_poll(x, m)                            /

       __wake_up(x, TASK_NORMAL, 1, (void *) (m))

#define wake_up_interruptible_poll(x, m)               /

       __wake_up(x, TASK_INTERRUPTIBLE, 1, (void *) (m))

这里的m值，应该和设备发生的事件相符合。设置poll_table_entry结构体的key项的函数是：

#define POLLIN_SET

(POLLRDNORM | POLLRDBAND | POLLIN | POLLHUP | POLLERR)

#define POLLOUT_SET (POLLWRBAND | POLLWRNORM | POLLOUT | POLLERR)

#define POLLEX_SET (POLLPRI)

static inline void wait_key_set(poll_table *wait, unsigned long in,

                            unsigned long out, unsigned long bit)

{

       if (wait) {

              wait->key = POLLEX_SET;

              if (in & bit)

                     wait->key |= POLLIN_SET;

              if (out & bit)

                     wait->key |= POLLOUT_SET;

       }

}

这里的m值，可以参考上面的宏来设置，注意传递的不是key的指针，而就是其值本身，只不过在wake_up_poll()到pollwake()的传递过程中是将其转换成指针的。

       如果唤醒函数使用后面一组的话，再加上合理设置key值，我相信pollwake()函数中的if一定会严格把关，不让应用程序没有监测的事件唤醒应用进程，从而避免了发生误唤醒。
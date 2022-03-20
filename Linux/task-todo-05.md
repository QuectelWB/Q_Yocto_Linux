#内核空间操作文件


1. 其实Linux Kernel组成员不赞成在kernel中独立的读写文件(这样做可能会影响到策略和安全问题)，对内核需要的文件内容，最好由应用层配合完成。

2. 在可加载的kernel module中使用这种方式读写文件可能使模块加载失败，原因是内核可能没有EXPORT你所需要的所有这些函数。

3. 分析以上某些函数的参数可以看出，这些函数的正确运行需要依赖于进程环境，因此，有些函数不能在中断的handle或Kernel中不属于任可进程的代码中执行，否则可能出现崩溃，要避免这种情况发生，可以在kernel中创建内核线程，将这些函数放在线程环境下执行(创建内核线程的方式请参数kernel_thread()函数)。


	------------------------------------------------------------------
	
	#include <linux/interrupt.h>
	#include<linux/irq.h>
	#include <linux/module.h>
	#include <linux/init.h>
	#include <linux/fs.h>
	#include <linux/fcntl.h>
	#include <linux/uaccess.h>

	MODULE_LICENSE("GPL");

	static char buf[] ="good study wang";

	static char buf1[10];

	static int __init test_init(void)
	{

		struct file *fp;
	
		mm_segment_t fs;

		loff_t pos;

		printk("test-file enter\n");

		fp = filp_open("/home/q/kernel_file", O_RDWR | O_CREAT, 0644);

		if(IS_ERR(fp)) {
			printk("create file error\n");
			return -1;
		}
	
	/**********************
	vfs_read和vfs_write函数中，其参数buf指向的用户空间的内存地址，如果我们直接使用内核空间的指针，则会返回-EFALUT
	改变内核对内存地址检查的处理方式
	***********************/
		fs = get_fs();
		set_fs(KERNEL_DS);
		pos = 0;
		vfs_write(fp, buf, sizeof(buf), &pos);
		pos = 0;
		vfs_read(fp, buf1, sizeof(buf), &pos);
		printk("read: %s\n", buf1);
		filp_close(fp, NULL);
		set_fs(fs);
		
		return 0;
	}
	
	void __exit test_exit(void)
	{
		printk("test-file exit\n");
	}
	
	module_init(test_init);
	module_exit(test_exit);
	
	---------------------------------------------------------
	



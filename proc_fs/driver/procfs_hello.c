#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define HELLO_PROC_FS_NODE_NAME "hello_proc"
static struct proc_dir_entry *hello_proc_dir_entry;

static int hello_read_proc(char *page, 
				           char **start, 
						   off_t off, 
						   int count, 
						   int *eof, 
						   void *data)
{
	count = sprintf(page, "%s", "hello_read_proc");
	*eof = 1;

	printk(KERN_ALERT"hello_read_proc\n");
	return 0;
}

static int hello_write_proc(struct file *file, 
				            const char __user *buffer,
							unsigned long count,
							void *data)
{
	int err;

	err = copy_to_user(buffer,"hello_write_proc", 16);
	if (err)
		printk(KERN_ALERT"Fail: hello_write_proc\n");
	return 0;
}

static int __init hello_proc_init(void)
{
	hello_proc_dir_entry = create_proc_entry(HELLO_PROC_FS_NODE_NAME, 0, NULL);

	if (!hello_proc_dir_entry) {
		hello_proc_dir_entry->read_proc = hello_read_proc;
		hello_proc_dir_entry->write_proc = hello_write_proc;
	}

	return 0;
}

static void __exit hello_proc_exit(void)
{
	remove_proc_entry(HELLO_PROC_FS_NODE_NAME, NULL);
}

module_init(hello_proc_init);
module_exit(hello_proc_exit);
MODULE_LICENSE("Dual BSD/GPL");


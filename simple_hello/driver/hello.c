#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void)
{
		int *p = 0;

		*p = 1; 
		return 0;
}

static void __exit hello_exit(void)
{
		return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");


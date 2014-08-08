#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#define CLASS_NAME "harvis-class"

static struct class *harvis_class = NULL;

static int __init class_test_init(void)
{
	harvis_class = class_create(THIS_MODULE, CLASS_NAME);	
	return 0;
}

static void __exit class_test_exit(void)
{
	if (harvis_class) 
		class_destroy(harvis_class);
}
module_init(class_test_init);
module_exit(class_test_exit);
MODULE_LICENSE("GPL");

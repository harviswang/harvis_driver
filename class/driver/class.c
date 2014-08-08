#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define CLASS_NAME  "harvis-class"
#define DEVICE_NAME "harvis-device"
#define NODE_NAME   "harvis-node"

static struct class *harvis_class = NULL;
static struct device *harvis_device = NULL;
static dev_t devno;

static int __init class_test_init(void)
{
	int err = -1;

	harvis_class = class_create(THIS_MODULE, CLASS_NAME);	
	if (IS_ERR(harvis_class)) {
		err = PTR_ERR(harvis_class);
		printk(KERN_ALERT"class_create() failed.\n");
		goto fail;
	}

	err = alloc_chrdev_region(&devno, 0, 1, NODE_NAME);
	if (err < 0) {
		printk(KERN_ALERT"alloc_chrdev_region failed.\n");
		goto fail;
	}

	harvis_device = device_create(harvis_class, NULL, devno, NULL, "harvis-dev");
	if (IS_ERR(harvis_device)) {
		err = PTR_ERR(harvis_device);
		printk(KERN_ALERT"device_create failed.\n");
		goto destroy_class;
	}

	return 0;

destroy_class:
	class_destroy(harvis_class);

fail:
	return err;
}

static void __exit class_test_exit(void)
{
	if (harvis_class) 
		class_destroy(harvis_class);

	if (harvis_device)
		device_destroy(harvis_class, devno);
}
module_init(class_test_init);
module_exit(class_test_exit);
MODULE_LICENSE("GPL");

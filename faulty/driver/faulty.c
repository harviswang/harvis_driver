#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static ssize_t faulty_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos);
static ssize_t faulty_read(struct file *filp, char __user *buf, size_t count, loff_t *pos);
static dev_t devno;
static struct cdev faulty_dev;
static struct class *faulty_class;

static struct file_operations faulty_fops = {
	.owner  = THIS_MODULE,
	.read   = faulty_read,
	.write  = faulty_write,
};

static ssize_t faulty_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	*(int *)0 = 0;
	return 0;
}

static ssize_t faulty_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	int ret;
	char stack_buf[4];

	/* occur a stack overflow */
	memset(stack_buf, 0xff, 20); /* actuall is 4, but we use 20 instead */
	if (count > 4)
		count = 4;

	ret = copy_to_user(buf, stack_buf, count);
	if (!ret)
		return count;

	return ret;
}

static int faulty_dev_setup(void)
{
	int ret;
	struct device *device;

	ret = alloc_chrdev_region(&devno, 0, 1, "faulty");
	if (ret < 0) {
		printk(KERN_ALERT"Failed to alloc_chrdev_region.\n");
		return ret;
	}

	memset(&faulty_dev, 0, sizeof(faulty_dev));
	cdev_init(&faulty_dev, &faulty_fops);
	faulty_dev.owner = THIS_MODULE;
	faulty_dev.ops   = &faulty_fops;

	ret = cdev_add(&faulty_dev, devno, 1);
	if (ret < 0) 
		return ret;

	faulty_class = class_create(THIS_MODULE, "faulty");
	if (IS_ERR(faulty_class)) 
		goto destroy_cdev;

	device = device_create(faulty_class, NULL, devno, NULL, "%s", "faulty");
	if (IS_ERR(device))
		goto destroy_class;
	
	return 0;
destroy_class:
	class_destroy(faulty_class);

destroy_cdev:
	cdev_del(&faulty_dev);

	return -1;
}

static int __init faulty_module_init(void)
{
	faulty_dev_setup();

	printk(KERN_ALERT"HZ = %d\n", HZ);

	return 0;
}

static void __exit faulty_module_exit(void)
{
	if (faulty_class) {
		device_destroy(faulty_class, devno);
		class_destroy(faulty_class);
	}

	cdev_del(&faulty_dev);

	unregister_chrdev_region(devno, 1);
}

module_init(faulty_module_init);
module_exit(faulty_module_exit);
MODULE_LICENSE("GPL");





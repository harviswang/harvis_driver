#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#define JIFFIES_64_DEVICE_NODE_NAME "jiffies_64"
#define JIFFIES_64_DEVICE_CLASS_NAME "jiffies_64"
#define JIFFIES_64_DEVICE_FILE_NAME "jiffies_64"

static unsigned long long value_of_jiffies_64;
static struct timer_list tm;
static dev_t devno = 0;
static struct class *jiffies_64_class = NULL;
static struct cdev *jiffies_64_dev = NULL;


static void timer_func(unsigned long data)
{
	data = data;
	memset(&value_of_jiffies_64, 0, sizeof(value_of_jiffies_64));
	value_of_jiffies_64 = get_jiffies_64();
	printk(KERN_ALERT "jiffies_64 is %lld\n", value_of_jiffies_64);
	mod_timer(&tm, jiffies + HZ);
}

static ssize_t jiffies_64_read(struct file *filp, char __user *buf, size_t size, loff_t *f_pos)
{
	if (buf == NULL || size != sizeof(value_of_jiffies_64))
		return -EINVAL;

	if (value_of_jiffies_64 == 0)
		value_of_jiffies_64 = get_jiffies_64();

	if (copy_to_user(buf, &value_of_jiffies_64, sizeof(value_of_jiffies_64))) 
		return -ENOMEM;

	memset(&value_of_jiffies_64, 0, sizeof(value_of_jiffies_64));

	return 0;
}

static ssize_t jiffies_64_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t jiffies_64_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations jiffies_64_fops = {
	.owner = THIS_MODULE,
	.open = jiffies_64_open,
	.read = jiffies_64_read,
	.release = jiffies_64_release,
};

static int __init jiffies_64_init(void)
{
	int err = -1;
	struct device *device = NULL;

	init_timer(&tm);
	tm.expires = jiffies + HZ;
	tm.function = timer_func;
	add_timer(&tm);

	err = alloc_chrdev_region(&devno, 0, 1, JIFFIES_64_DEVICE_NODE_NAME);
	if (err < 0) {
		printk(KERN_ALERT"Failed to alloc char dev region.\n");
		goto fail;
	}

	jiffies_64_dev = (struct cdev *)kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if (!jiffies_64_dev) {
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc jiffies_64_dev\n");
		goto unregister;
	}

	/* setup cdev */
	cdev_init(jiffies_64_dev, &jiffies_64_fops);
	jiffies_64_dev->owner = THIS_MODULE;
	jiffies_64_dev->ops   = &jiffies_64_fops;
	err = cdev_add(jiffies_64_dev, devno, 1);
	if (err)
		return err;

	jiffies_64_class = class_create(THIS_MODULE, JIFFIES_64_DEVICE_CLASS_NAME);
	if (IS_ERR(jiffies_64_class)) {
		err = PTR_ERR(jiffies_64_class);
		printk(KERN_ALERT"Failed to create jiffies_64 class\n");
		goto destroy_cdev;
	}

	device = device_create(jiffies_64_class, NULL, devno, "%s", JIFFIES_64_DEVICE_FILE_NAME);
	if (IS_ERR(device)) {
		err = PTR_ERR(device);
		printk(KERN_ALERT"Failed to create jiffies_64 device.\n");
		goto destroy_class;
	}

destroy_class:
	class_destroy(jiffies_64_class);

destroy_cdev:
	cdev_del(jiffies_64_dev);

unregister:
	unregister_chrdev_region(devno, 1);

fail:
	return err;
}

static void __exit jiffies_64_exit(void)
{
	del_timer(&tm);
	printk(KERN_ALERT"line %d", __LINE__);
	if (jiffies_64_class) {
	printk(KERN_ALERT"line %d", __LINE__);
		device_destroy(jiffies_64_class, devno);
	printk(KERN_ALERT"line %d", __LINE__);
		class_destroy(jiffies_64_class);
	printk(KERN_ALERT"line %d", __LINE__);
	}

	printk(KERN_ALERT"line %d", __LINE__);
	if (jiffies_64_dev)  {
	printk(KERN_ALERT"line %d", __LINE__);
		cdev_del(jiffies_64_dev);
	printk(KERN_ALERT"line %d", __LINE__);
		kfree(jiffies_64_dev);
	printk(KERN_ALERT"line %d", __LINE__);
	}

	printk(KERN_ALERT"line %d", __LINE__);
	unregister_chrdev_region(devno, 1);
	printk(KERN_ALERT"line %d", __LINE__);
}

module_init(jiffies_64_init);
module_exit(jiffies_64_exit);
MODULE_LICENSE("GPL");


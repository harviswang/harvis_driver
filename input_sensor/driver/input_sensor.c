#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#include <asm/uaccess.h>
#include <asm/irq.h>

#include <mach/hardware.h>

#define DEVICE_NAME "input-sensor"
#define SENSOR_PIN_SETTING 1
#ifndef JITTER
#define JITTER
#endif

struct sensor_irq_desc {
	int irq;
	int pin;
	int pin_setting;
	int number;
	char *name;
};

static struct sensor_irq_desc sensor_irqs [] = {
	{AT91_PIN_PB1,  AT91_PIN_PB1,  SENSOR_PIN_SETTING, 0, "LEVEL_UP"},
	{AT91_PIN_PB0,  AT91_PIN_PB0,  SENSOR_PIN_SETTING, 1, "LEVEL_DOWNLOAD"},
	{AT91_PIN_PC7,  AT91_PIN_PC7,  SENSOR_PIN_SETTING, 2, "DOOR"},
	{AT91_PIN_PC29, AT91_PIN_PC29, SENSOR_PIN_SETTING, 3, "INFRARED"},
	{AT91_PIN_PC22, AT91_PIN_PC22, SENSOR_PIN_SETTING, 4, "BASE"},
	{AT91_PIN_PC24, AT91_PIN_PC24, SENSOR_PIN_SETTING, 5, "MICROWAVE"},
};

static unsigned char sensor_values [] = {0, 0, 0, 0, 0, 0};
static DECLARE_WAIT_QUEUE_HEAD(sensor_waitq);
static volatile int ev_press = 0;
static struct sensor_irq_desc *sensor_irq = NULL;
#ifdef JITTER
static struct timer_list sensor_timer;
static spinlock_t sensor_spinlock;
#endif

static irqreturn_t sensor_interrupt(int irq, void *dev_id)
{
#ifdef JITTER
	int err;

	sensor_irq = (struct sensor_irq_desc *)dev_id;
	err = mod_timer(&sensor_timer, jiffies + (HZ / 50));
	if (err == 1)
		printk(KERN_ALERT"Active timer: mod_timer() = %d\n", err);
	else
		printk(KERN_ALERT"Inactive timer: mod_timer() = %d\n", err);
#else
	int down;
	sensor_irq = (struct sensor_irq_desc *)dev_id;

	down = !gpio_get_value(sensor_irq->pin);/* in schema the default is high level */
	if (down != (sensor_values[sensor_irq->number])) {
		ev_press = 1;
		sensor_values[sensor_irq->number] = down;
		wake_up_interruptible(&sensor_waitq);
	}
#endif
	return IRQ_RETVAL(IRQ_HANDLED);
}

#ifdef JITTER
static void sensor_timer_function(unsigned long data)
{
	int down;
	struct sensor_irq_desc *sensor_irq_tmp = *(struct sensor_irq_desc **)data;

	if (sensor_irq_tmp == NULL) /* when it first funt, sensor_irq is NULL */
		return;

	down = !gpio_get_value(sensor_irq_tmp->pin);/* in schema the default is high level */
	if (down != (sensor_values[sensor_irq_tmp->number])) {
		ev_press = 1;
		sensor_values[sensor_irq_tmp->number] = down;
		wake_up_interruptible(&sensor_waitq);
	}

}
#endif

static int sensor_open(struct inode *inode, struct file *file)
{
	int i;
	int err = 0;

	for (i = 0; i < ARRAY_SIZE(sensor_irqs); i++) {
		if (sensor_irqs[i].irq < 0)
			continue;
		
		printk("sensor_open:i =  %d\n", i);

		at91_set_gpio_input(sensor_irqs[i].pin, sensor_irqs[i].pin_setting);
		at91_set_deglitch(sensor_irqs[i].pin, sensor_irqs[i].pin_setting);

/*		err = request_irq(sensor_irqs[i].irq, sensor_interrupt, IRQ_TYPE_EDGE_BOTH, sensor_irqs[i].name, (void *)&sensor_irqs[i]); */
		err = request_irq(sensor_irqs[i].irq, sensor_interrupt, IRQ_TYPE_EDGE_BOTH, sensor_irqs[i].name, (void *)&sensor_irqs[i]);
		if (err) {
			printk("input_sensor[%d] request_irq failed\n", i);
			break;
		}
	}

	if (err) {
		i--;
		for (; i >= 0; i--) {
			if (sensor_irqs[i].irq < 0)
				continue;

			disable_irq(sensor_irqs[i].irq);
			free_irq(sensor_irqs[i].irq, (void *)&sensor_irqs[i]);
		}

		return -EBUSY;
	}

	ev_press = 1;
	return 0;
}

static int sensor_release(struct inode *inode, struct file *file)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(sensor_irqs); i++) {
		if (sensor_irqs[i].irq < 0) 
			continue;

		disable_irq(sensor_irqs[i].irq);
		free_irq(sensor_irqs[i].irq, (void *)&sensor_irqs[i]);
	}

	return 0;
}

static int sensor_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	unsigned long err;

	if (!ev_press) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		wait_event_interruptible(sensor_waitq, ev_press);
	}
	ev_press = 0;
	
	err = copy_to_user(buff, (void *)sensor_values, min(sizeof(sensor_values), count));

	return err ? -EFAULT : min(sizeof(sensor_values), count);
}

static unsigned int sensor_poll(struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;

	poll_wait(file, &sensor_waitq, wait);
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open  = sensor_open,
	.release = sensor_release,
	.read  = sensor_read,
	.poll  = sensor_poll,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DEVICE_NAME,
	.fops  = &dev_fops,
};

static int __init sensor_module_init(void)
{
	int ret;
#ifdef JITTER	
	memset(&sensor_timer, 0, sizeof(sensor_timer));
	init_timer(&sensor_timer);
	spin_lock_init(&sensor_spinlock);

	spin_lock(&sensor_spinlock);
	sensor_timer.function = sensor_timer_function;
	sensor_timer.data     = (unsigned long)&sensor_irq;
	sensor_timer.expires  = jiffies + (HZ / 50);
	spin_unlock(&sensor_spinlock);
	
	add_timer(&sensor_timer);
#endif

	ret = misc_register(&misc);
	printk(KERN_ALERT"%s\tinitialized\n", DEVICE_NAME);

	return ret;
}

static void __exit sensor_module_exit(void)
{
	misc_deregister(&misc);

#ifdef JITTER
	del_timer(&sensor_timer);
#endif
}

module_init(sensor_module_init);
module_exit(sensor_module_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Harvis Wang <jiankangshiye@aliyun.com");



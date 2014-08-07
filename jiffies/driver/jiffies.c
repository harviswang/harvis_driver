#include <linux/init.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

static struct timer_list jiffies_timer;

static void jiffies_timer_func(unsigned long data)
{
	printk(KERN_ALERT"jiffies=%ld\n", jiffies);
	mod_timer(&jiffies_timer, jiffies + HZ);
}
static int __init jiffies_init(void)
{
	init_timer(&jiffies_timer);
	jiffies_timer.function = jiffies_timer_func;
	jiffies_timer.expires  = jiffies + HZ;
	add_timer(&jiffies_timer);

	printk(KERN_ALERT"The address of jiffies is %8x\n", (unsigned int)&jiffies);
	return 0;
}

static void  __exit jiffies_exit(void)
{
	del_timer(&jiffies_timer);
}

module_init(jiffies_init);
module_exit(jiffies_exit);


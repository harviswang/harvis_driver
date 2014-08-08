#include <linux/timer.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

struct timer_list tm;
static int num;
static void func()
{
	num++;
	mod_timer(&tm, jiffies + HZ);

	printk("Hello, timer :%d\n", num);
}

static int timer_init(void)
{
	num = 0;

	init_timer(&tm);
	tm.expires = jiffies + HZ;
	tm.function = func;
	add_timer(&tm);

	return 0;
}

static void timer_exit(void)
{
	del_timer(&tm);

	printk("remove timer\n");
}

module_init(timer_init);
module_exit(timer_exit);


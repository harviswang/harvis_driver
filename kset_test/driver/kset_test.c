#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct kset kset_p;
static struct kset kset_c;

static int kset_filter(struct kset *kset, struct kobject *kobj)
{
	printk("Filter kobj %s.\n", kobj->name);
	return 1;
}

static const char *kset_name(struct kset *kset, struct kobject *kobj)
{
	static char buf[20];

	printk(KERN_ALERT"Name: kobj %s.\n", kobj->name);
	sprintf(buf, "%s", "kset_name");

	return buf;
}

static int kset_uevent(struct kset *kset, struct kobject *kobj, struct kobj_uevent_env *env)
{
	int i = 0;

	printk("uevent: kobj %s.\n", kobj->name);

	while (i < env->envp_idx) {
		printk("%s.\n", env->envp[i]);
		i++;
	}

	return 0;
}

static struct kset_uevent_ops uevent_ops = {
	.filter = kset_filter,
	.name   = kset_name,
	.uevent = kset_uevent,
};
static struct kobj_type my_kobj_type; /* 规避kernel oops bug */
static int __init kset_test_init(void)
{
	int err = 0;

	printk(KERN_ALERT"kset test init.\n");
	kobject_set_name(&kset_p.kobj, "kset_p");
	kset_p.uevent_ops = &uevent_ops;
	kset_p.kobj.ktype = &my_kobj_type;
	err = kset_register(&kset_p);
	if (err)
		return err;

	kobject_set_name(&kset_c.kobj, "kset_c");
	kset_c.uevent_ops = &uevent_ops;
	kset_c.kobj.ktype = &my_kobj_type;
	err = kset_register(&kset_c);
	if (err)
		return err;

	return 0;
}

static void kset_test_exit(void)
{
	printk(KERN_ALERT"kset test exit.\n");
	kset_unregister(&kset_p);
	kset_unregister(&kset_c);
}

module_init(kset_test_init);
module_exit(kset_test_exit);
MODULE_LICENSE("GPL");


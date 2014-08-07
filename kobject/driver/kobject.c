#include <linux/init.h>
#include <linux/module.h>

static struct kobject *kobj;
static struct kobj_type *ktype;

static int __init kobject_test_init(void)
{
	kobj = kobject_create();
	if (kobj == NULL)
		return -ENOMEM;

	kobject_init(kobj, NULL);
	return 0;
}

static void __exit kobject_test_exit(void)
{
	
}

module_init(kobject_test_init);
module_exit(kobject_test_exit);

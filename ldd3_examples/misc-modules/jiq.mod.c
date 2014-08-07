#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x41086e, "module_layout" },
	{ 0x8bd5b603, "param_get_long" },
	{ 0x3457cb68, "param_set_long" },
	{ 0xc633495b, "schedule_work" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0x4292364c, "schedule" },
	{ 0x91009452, "schedule_delayed_work" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xe83fea1, "del_timer_sync" },
	{ 0x46085e4f, "add_timer" },
	{ 0x6a9f26c9, "init_timer_key" },
	{ 0xeb1fabf6, "interruptible_sleep_on" },
	{ 0xf397b9aa, "__tasklet_schedule" },
	{ 0x642e54ac, "__wake_up" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x7d0bea0b, "per_cpu__current_task" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0x7d11c268, "jiffies" },
	{ 0x70b2af54, "create_proc_entry" },
	{ 0x8d8aacb9, "remove_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D13D8F6452D526FDA50AD3C");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};

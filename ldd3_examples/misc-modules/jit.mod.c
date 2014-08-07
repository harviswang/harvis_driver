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
	{ 0x6980fe91, "param_get_int" },
	{ 0xff964b25, "param_set_int" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x67053080, "current_kernel_time" },
	{ 0x1d2e87c6, "do_gettimeofday" },
	{ 0x37e74642, "get_jiffies_64" },
	{ 0x46085e4f, "add_timer" },
	{ 0x6a9f26c9, "init_timer_key" },
	{ 0x642e54ac, "__wake_up" },
	{ 0xf397b9aa, "__tasklet_schedule" },
	{ 0x267fc65b, "__tasklet_hi_schedule" },
	{ 0x37a0cba, "kfree" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0x4292364c, "schedule" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xa5808bbf, "tasklet_init" },
	{ 0x7d0bea0b, "per_cpu__current_task" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xffc7c184, "__init_waitqueue_head" },
	{ 0x884f145, "kmem_cache_alloc_trace" },
	{ 0x7d11c268, "jiffies" },
	{ 0xc288f8ce, "malloc_sizes" },
	{ 0x70b2af54, "create_proc_entry" },
	{ 0x8d8aacb9, "remove_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FDD818A2565042FE2DA5160");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};

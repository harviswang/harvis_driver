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
	{ 0x581bf245, "seq_release" },
	{ 0x2b084b2b, "seq_read" },
	{ 0xa54015e2, "seq_lseek" },
	{ 0x884f145, "kmem_cache_alloc_trace" },
	{ 0xc288f8ce, "malloc_sizes" },
	{ 0x37a0cba, "kfree" },
	{ 0x79566c2d, "seq_printf" },
	{ 0x3d8e39e5, "seq_open" },
	{ 0x70b2af54, "create_proc_entry" },
	{ 0x8d8aacb9, "remove_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BCEFFA8CB4F6FFCC551CD45");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};

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
	{ 0xf10de535, "ioread8" },
	{ 0xc5534d64, "ioread16" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xe484e35f, "ioread32" },
	{ 0x727c4f3, "iowrite8" },
	{ 0x37a0cba, "kfree" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x7dceceac, "capable" },
	{ 0xb72397d5, "printk" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0xcfdbba1a, "__register_chrdev" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xedc03953, "iounmap" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "128783BB606195AC0D6F863");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};

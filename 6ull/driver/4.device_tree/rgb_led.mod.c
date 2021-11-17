#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x10d06f3b, "module_layout" },
	{ 0xe5135656, "platform_driver_unregister" },
	{ 0xf7d082ff, "cdev_del" },
	{ 0xa3f23a4d, "class_destroy" },
	{ 0x676c0ac6, "device_destroy" },
	{ 0xedc03953, "iounmap" },
	{ 0x911060d2, "__platform_driver_register" },
	{ 0x7772b225, "device_create" },
	{ 0xb489cb8d, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x99d31bfb, "cdev_add" },
	{ 0xb9aea4e3, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x981529cb, "of_iomap" },
	{ 0x848f5838, "of_find_node_by_name" },
	{ 0x5736a32a, "of_find_node_opts_by_path" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x5f754e5a, "memset" },
	{ 0x1e047854, "warn_slowpath_fmt" },
	{ 0x822137e2, "arm_heavy_mb" },
	{ 0x28cc25db, "arm_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x7c32d0f0, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2F69F52BD93076CE9D5A66C");

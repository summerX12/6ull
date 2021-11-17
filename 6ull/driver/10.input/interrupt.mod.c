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
	{ 0x7c4b72f, "input_unregister_device" },
	{ 0x4b52df2c, "input_register_device" },
	{ 0x2fea72c1, "input_set_capability" },
	{ 0x5dfa6817, "input_allocate_device" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0xfee51f12, "irq_of_parse_and_map" },
	{ 0x7ed47eaa, "gpiod_direction_input" },
	{ 0xfe990052, "gpio_free" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x470efb25, "of_get_named_gpio_flags" },
	{ 0x5736a32a, "of_find_node_opts_by_path" },
	{ 0x7c32d0f0, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x288e0e0f, "input_event" },
	{ 0xb06f864f, "gpiod_get_raw_value" },
	{ 0xe027f59b, "gpio_to_desc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EC541BBD7DCBF7A4EBFF625");

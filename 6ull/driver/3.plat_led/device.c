#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define CCM_CCGR1				0x020C406C
#define IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04	0x020E006C
#define IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO04	0x020E02F8
#define GPIO1_GDIR				0x0209C004
#define GPIO1_DR				0x0209C000

#define CCM_CCGR3				0x020C4074
#define GPIO4_GDIR				0x020A8004
#define GPIO4_DR				0x020A8000

#define IOMUXC_SW_MUX_CTL_PAD_GPIO4_IO020	0x020E01E0
#define IOMUXC_SW_PAD_CTL_PAD_GPIO4_IO020	0x020E046C

#define IOMUXC_SW_MUX_CTL_PAD_GPIO4_IO019	0x020E01DC
#define IOMUXC_SW_PAD_CTL_PAD_GPIO4_IO019	0x020E0468

struct resource rled_resource[] = 
{
	[0] = DEFINE_RES_MEM(GPIO1_DR,4),
	[1] = DEFINE_RES_MEM(GPIO1_GDIR,4),
	[2] = DEFINE_RES_MEM(IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO04,4),
	[3] = DEFINE_RES_MEM(CCM_CCGR1,4),
	[4] = DEFINE_RES_MEM(IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO04,4),
};

struct resource gled_resource[] = 
{
	[0] = DEFINE_RES_MEM(GPIO4_DR,4),
	[1] = DEFINE_RES_MEM(GPIO4_GDIR,4),
	[2] = DEFINE_RES_MEM(IOMUXC_SW_MUX_CTL_PAD_GPIO4_IO020,4),
	[3] = DEFINE_RES_MEM(CCM_CCGR3,4),
	[4] = DEFINE_RES_MEM(IOMUXC_SW_PAD_CTL_PAD_GPIO4_IO020,4),
};

struct resource bled_resource[] = 
{
	[0] = DEFINE_RES_MEM(GPIO4_DR,4),
	[1] = DEFINE_RES_MEM(GPIO4_GDIR,4),
	[2] = DEFINE_RES_MEM(IOMUXC_SW_MUX_CTL_PAD_GPIO4_IO019,4),
	[3] = DEFINE_RES_MEM(CCM_CCGR3,4),
	[4] = DEFINE_RES_MEM(IOMUXC_SW_PAD_CTL_PAD_GPIO4_IO019,4),
};

static void led_release(struct device *dev)
{
	
}

unsigned int rled_hwinfo[2] = {4,26};
unsigned int gled_hwinfo[2] = {20,12};
unsigned int bled_hwinfo[2] = {19,12};

static struct platform_device rled = 
{
	.name = "myled",
	.id = 0,
	.num_resources = ARRAY_SIZE(rled_resource),
	.resource = rled_resource,
	.dev = {
		.release = led_release,
		.platform_data = rled_hwinfo,
		},
};

static struct platform_device gled =
{
	.name = "myled",
	.id = 1,
	.num_resources = ARRAY_SIZE(gled_resource),
	.resource = gled_resource,
	.dev = {
		.release = led_release,
		.platform_data = gled_hwinfo,
		},
};
static struct platform_device bled =
{
	.name = "myled",
	.id = 2,
	.num_resources = ARRAY_SIZE(bled_resource),
	.resource = bled_resource,
	.dev = {
		.release = led_release,
		.platform_data = bled_hwinfo,
		},
};


static __init int led_dev_init(void)
{
	printk("led dev init\n");

	platform_device_register(&rled);
	platform_device_register(&gled);
	platform_device_register(&bled);

	return 0;
}
module_init(led_dev_init);

static __exit void led_dev_exit(void)
{
	printk("led dev exit\n");

	platform_device_unregister(&rled);
	platform_device_unregister(&gled);
	platform_device_unregister(&bled);
	
}
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");

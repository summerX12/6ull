#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <asm/mach/map.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/io.h>
#include <linux/device.h>

#include <linux/platform_device.h>

#define DEV_NAME	"rgb_led"
#define DEV_CNT		(1)


struct led_resource
{
	struct device_node *device_node;
	void __iomem *virtual_CCM_CCGR;
	void __iomem *virtual_IOMUXC_SW_MUX_CTL_PAD;
	void __iomem *virtual_IOMUXC_SW_PAD_CTL_PAD;
	void __iomem *virtual_DR;
	void __iomem *virtual_GDIR;
};

static dev_t led_devno;
static struct cdev led_chr_dev;
struct class *class_led;
struct device *device;
struct device_node *rgb_led_device_node;

struct led_resource led_red;
struct led_resource led_blue;
struct led_resource led_green;

static int led_chr_dev_open(struct inode *inode,struct file *filp)
{
	printk("\nopen form driver\n");
	return 0;
}

static ssize_t led_chr_dev_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
{
	unsigned int register_data = 0;
	unsigned char write_data;

	int error = copy_from_user(&write_data,buf,cnt);
	if(error < 0)
	{
		return -1;
	}

	if(write_data & 0x04)//GPIO1_04
	{
		register_data = readl(led_red.virtual_DR);
		register_data &= ~(0x01 << 4);
		writel(register_data,led_red.virtual_DR);
	}
	else
	{
		register_data = readl(led_red.virtual_DR);
		register_data |= (0x01 << 4);
		writel(register_data,led_red.virtual_DR);
	}

	if(write_data & 0x02)//GPIO04_20
	{
		register_data = readl(led_green.virtual_DR);
		register_data &= ~(0x01 << 20);
		writel(register_data,led_green.virtual_DR);
	}
	else
	{
		register_data = readl(led_green.virtual_DR);
		register_data &= ~(0x01 << 20);
		writel(register_data,led_green.virtual_DR);

	}

	if(write_data & 0x01)
	{
		register_data = readl(led_blue.virtual_DR);
		register_data &= ~(0x01 << 19);
		writel(register_data,led_blue.virtual_DR);
	}
	else
	{
		register_data = readl(led_blue.virtual_DR);
		register_data &= ~(0x01 << 19);
		writel(register_data,led_blue.virtual_DR);
	}

	return 0;
}

static struct file_operations led_chr_dev_fops = 
{
	.owner = THIS_MODULE,
	.open = led_chr_dev_open,
	.write = led_chr_dev_write,
};



static int led_probe(struct platform_device *pdev)
{
	int ret = -1;
	unsigned int register_data = 0;

	printk(KERN_EMERG "\t match succedd \n");

	rgb_led_device_node = of_find_node_by_path("/rgb_led");
	if(rgb_led_device_node == NULL)
	{
		printk(KERN_ERR "\t grt rgb_led failed! \n");
		return -1;
	}

////red
	led_red.device_node = of_find_node_by_name(rgb_led_device_node,"rgb_led_red");
	if(led_red.device_node == NULL)
	{
		printk(KERN_ERR "\nget rgb_led_red_device_node failed!\n");
		return -1;
	}

	led_red.virtual_CCM_CCGR = of_iomap(led_red.device_node,0);
	led_red.virtual_IOMUXC_SW_MUX_CTL_PAD = of_iomap(led_red.device_node,1);
	led_red.virtual_IOMUXC_SW_PAD_CTL_PAD = of_iomap(led_red.device_node,2);
	led_red.virtual_DR = of_iomap(led_red.device_node,3);
	led_red.virtual_GDIR = of_iomap(led_red.device_node,4);

	
	register_data = readl(led_red.virtual_CCM_CCGR);
	register_data |= (0x03 << 26);
	writel(register_data,led_red.virtual_CCM_CCGR);

	register_data = readl(led_red.virtual_IOMUXC_SW_MUX_CTL_PAD);
	register_data &= ~(0xff << 0);
	register_data |= (0x05 << 0);
	writel(register_data,led_red.virtual_IOMUXC_SW_MUX_CTL_PAD);

	register_data = readl(led_red.virtual_IOMUXC_SW_PAD_CTL_PAD);
	register_data |= (0x10B0);
	writel(register_data,led_red.virtual_IOMUXC_SW_PAD_CTL_PAD);

	register_data = readl(led_red.virtual_GDIR);
	register_data |= (0x01 << 4);
	writel(register_data,led_red.virtual_GDIR);

	register_data = readl(led_red.virtual_DR);
	register_data |= (0x01 << 4);
	writel(register_data,led_red.virtual_DR);

///blue
	led_blue.device_node = of_find_node_by_name(rgb_led_device_node,"rgb_led_blue");
	if(led_blue.device_node == NULL)
	{
		printk(KERN_ERR "\nget rgb_led_blue_device_node failed!\n");
		return -1;
	}

	led_blue.virtual_CCM_CCGR = of_iomap(led_blue.device_node,0);
	led_blue.virtual_IOMUXC_SW_MUX_CTL_PAD = of_iomap(led_blue.device_node,1);
	led_blue.virtual_IOMUXC_SW_PAD_CTL_PAD = of_iomap(led_blue.device_node,2);
	led_blue.virtual_DR = of_iomap(led_blue.device_node,3);
	led_blue.virtual_GDIR = of_iomap(led_blue.device_node,4);

	
	register_data = readl(led_blue.virtual_CCM_CCGR);
	register_data |= (0x03 << 12);
	writel(register_data,led_blue.virtual_CCM_CCGR);

	register_data = readl(led_blue.virtual_IOMUXC_SW_MUX_CTL_PAD);
	register_data &= ~(0xff << 0);
	register_data |= (0x05 << 0);
	writel(register_data,led_blue.virtual_IOMUXC_SW_MUX_CTL_PAD);

	register_data = readl(led_blue.virtual_IOMUXC_SW_PAD_CTL_PAD);
	register_data = (0x10B0);
	writel(register_data,led_blue.virtual_IOMUXC_SW_PAD_CTL_PAD);
	
	register_data = readl(led_blue.virtual_GDIR);
	register_data |= (0x01 << 19);
	writel(register_data,led_blue.virtual_GDIR);

	register_data = readl(led_blue.virtual_DR);
	register_data |= (0x01 << 19);
	writel(register_data,led_blue.virtual_DR);

///green
	led_green.device_node = of_find_node_by_name(rgb_led_device_node,"rgb_led_green");
	if(led_green.device_node == NULL)
	{
		printk("\nfind led_green_device_node fail\n");
		return -1;
	}

	led_green.virtual_CCM_CCGR = of_iomap(led_green.device_node,0);
	led_green.virtual_IOMUXC_SW_MUX_CTL_PAD = of_iomap(led_green.device_node,1);
	led_green.virtual_IOMUXC_SW_PAD_CTL_PAD = of_iomap(led_green.device_node,2);
	led_green.virtual_DR = of_iomap(led_green.device_node,3);
	led_green.virtual_GDIR = of_iomap(led_green.device_node,4);

	register_data = readl(led_green.virtual_CCM_CCGR);
	register_data |= (0x03 << 12);
	writel(register_data,led_green.virtual_CCM_CCGR);

	register_data = readl(led_green.virtual_IOMUXC_SW_MUX_CTL_PAD);
	register_data &= ~(0xff << 0);
	register_data |= (0x05 << 0);
	writel(register_data,led_green.virtual_IOMUXC_SW_MUX_CTL_PAD);

	register_data = readl(led_green.virtual_IOMUXC_SW_PAD_CTL_PAD);
	register_data = (0x10B0);
	writel(register_data,led_green.virtual_IOMUXC_SW_PAD_CTL_PAD);

	register_data = readl(led_green.virtual_GDIR);
	register_data |= (0x01 << 20);
	writel(register_data,led_green.virtual_GDIR);

	register_data = readl(led_green.virtual_DR);
	register_data |= (0x01 << 20);
	writel(register_data,led_green.virtual_DR);


///cdev 
	ret = alloc_chrdev_region(&led_devno,0,DEV_CNT,DEV_NAME);
	if(ret < 0)
	{
		printk("fail to alloc led_devno\n");
		goto alloc_err;
	}
	led_chr_dev.owner = THIS_MODULE;
	cdev_init(&led_chr_dev,&led_chr_dev_fops);
	ret = cdev_add(&led_chr_dev,led_devno,DEV_CNT);
	if(ret < 0)
	{
		printk("fail to add cdev\n");
		goto add_err;
	}
	class_led = class_create(THIS_MODULE,DEV_NAME);
	device = device_create(class_led,NULL,led_devno,NULL,DEV_NAME);
	return 0;

add_err:
	unregister_chrdev_region(led_devno,DEV_CNT);
	printk("\n erron \n");
alloc_err:

	return -1;
}

static const struct of_device_id rgb_led[] = 
{
	{.compatible = "rgb_led"},
	{}
};

struct platform_driver led_platform_driver = 
{
	.probe = led_probe,
	.driver = {
		.name = "rgb-leds-platform",
		.owner = THIS_MODULE,
		.of_match_table = rgb_led,
	}
};


static int __init led_platform_driver_init(void)
{
	int DriverState;
	DriverState = platform_driver_register(&led_platform_driver);
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}
module_init(led_platform_driver_init);

static void __exit led_platform_driver_exit(void)
{
	iounmap(led_green.virtual_CCM_CCGR);
	iounmap(led_green.virtual_IOMUXC_SW_MUX_CTL_PAD);
	iounmap(led_green.virtual_IOMUXC_SW_PAD_CTL_PAD);
	iounmap(led_green.virtual_DR);
	iounmap(led_green.virtual_GDIR);

	iounmap(led_blue.virtual_CCM_CCGR);
	iounmap(led_blue.virtual_IOMUXC_SW_MUX_CTL_PAD);
	iounmap(led_blue.virtual_IOMUXC_SW_PAD_CTL_PAD);
	iounmap(led_blue.virtual_DR);
	iounmap(led_blue.virtual_GDIR);

	iounmap(led_red.virtual_CCM_CCGR);
	iounmap(led_red.virtual_IOMUXC_SW_MUX_CTL_PAD);
	iounmap(led_red.virtual_IOMUXC_SW_PAD_CTL_PAD);
	iounmap(led_red.virtual_DR);
	iounmap(led_red.virtual_GDIR);


	device_destroy(class_led,led_devno);
	class_destroy(class_led);
	cdev_del(&led_chr_dev);
	unregister_chrdev_region(led_devno,DEV_CNT);
	platform_driver_unregister(&led_platform_driver);

	printk(KERN_EMERG "exit\n");

}
module_exit(led_platform_driver_exit);

MODULE_LICENSE("GPL");

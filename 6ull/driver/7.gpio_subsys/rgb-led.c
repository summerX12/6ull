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


#define DEV_NAME    "rgb-leds"
#define DEV_CNT     (1)


static dev_t led_devno;
static struct cdev led_chr_dev;

struct class *class_led;
struct device *device;
struct device_node *rgb_led_device_node;

int rgb_led_red;
int rgb_led_green;
int rgb_led_blue;


static int led_cdev_open(struct inode *inode,struct file *filp)
{
    printk("\nopen driver\n");
    return 0;
}

static ssize_t led_cdev_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
{
    unsigned char write_data;

    int error = copy_from_user(&write_data,buf,cnt);
    if(error < 0)
    {
        return -1;
    }

    if(write_data & 0x04)//gpio1_04
    {
        gpio_direction_output(rgb_led_red,0);
    }
    else
    {
        gpio_direction_output(rgb_led_red,1);
    }

    if(write_data & 0x02)//gpio4_20
    {
        gpio_direction_output(rgb_led_green,0);
    }
    else
    {
        gpio_direction_output(rgb_led_green,1);
    }

    if(write_data & 0x01)//gpio4_19
    {
        gpio_direction_output(rgb_led_blue,0);
    }
    else
    {
        gpio_direction_output(rgb_led_blue,1);
    }
    return 0;
}

static struct file_operations led_cdev_ops = 
{
    .owner = THIS_MODULE,
    .open = led_cdev_open,
    .write = led_cdev_write,
};

static int led_probe(struct platform_device *pdv)
{
    int ret = 0;

    printk(KERN_EMERG "\t match successed\n");

    rgb_led_device_node = of_find_node_by_path("/rgb_led");
    if(rgb_led_device_node == NULL)
    {
        printk(KERN_EMERG "\t  get rgb_led failed!  \n");
    }

    rgb_led_red = of_get_named_gpio(rgb_led_device_node,"rgb_led_red",0);
    rgb_led_green=of_get_named_gpio(rgb_led_device_node,"rgb_led_green",0);
    rgb_led_blue=of_get_named_gpio(rgb_led_device_node,"rgb_led_blue",0);

    printk("rgb_led_red = %d,\n rgb_led_green = %d,\n rgb_led_blue = %d,\n", rgb_led_red,rgb_led_green,rgb_led_blue);

    gpio_direction_output(rgb_led_red,1);
    gpio_direction_output(rgb_led_green,1);
    gpio_direction_output(rgb_led_blue,1);



    ret = alloc_chrdev_region(&led_devno,0,DEV_CNT,DEV_NAME);
    if(ret < 0)
    {
        printk("fail to alloc led_devno");
        goto alloc_err;
    }
    led_chr_dev.owner = THIS_MODULE;
    cdev_init(&led_chr_dev,&led_cdev_ops);
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
    printk("\n error!\n");
alloc_err:
    return -1;
}

static const struct of_device_id rgb_led[] = 
{
    { .compatible = "fire,rgb-led"},
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
    int state;

    state = platform_driver_register(&led_platform_driver);
    printk(KERN_EMERG "\tDriverState is %d\n",state);
    
    return 0;
}
module_init(led_platform_driver_init);

static void __exit led_platform_driver_exit(void)
{
    platform_driver_unregister(&led_platform_driver);
    printk("led platform driver exit\n");
}
module_exit(led_platform_driver_exit);

MODULE_LICENSE("GPL");




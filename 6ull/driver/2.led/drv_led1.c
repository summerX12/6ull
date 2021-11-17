#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define DEV_NUM		(3)
#define DEV_NAME	"led_chrdev"

static dev_t devno;
struct class *led_class;

struct drive_led
{
	struct cdev dev;

	unsigned int __iomem *va_dr;
	unsigned int __iomem *va_gdir;
	unsigned int __iomem *va_iomuxc_mux;
	unsigned int __iomem *va_ccm_ccgrx;
	unsigned int __iomem *va_iomux_pad;

	unsigned long pa_dr;
	unsigned long pa_gdir;
	unsigned long pa_iomuxc_mux;
	unsigned long pa_ccm_ccgrx;
	unsigned long pa_iomux_pad;

	unsigned int led_pin;
	unsigned int clock_offset;
};


static int led_chrdev_open(struct inode *inode,struct file *filp)
{
	unsigned int val = 0;
	struct drive_led *led_cdev = (struct drive_led *)container_of(inode->i_cdev,struct drive_led,dev);

	filp->private_data = container_of(inode->i_cdev,struct drive_led,dev);

	printk("open\n");

	led_cdev->va_dr = ioremap(led_cdev->pa_dr,4);
	led_cdev->va_gdir = ioremap(led_cdev->pa_gdir,4);
	led_cdev->va_iomuxc_mux = ioremap(led_cdev->pa_iomuxc_mux,4);
	led_cdev->va_ccm_ccgrx = ioremap(led_cdev->pa_ccm_ccgrx,4);
	led_cdev->va_iomux_pad = ioremap(led_cdev->pa_iomux_pad,4);

	val = ioread32(led_cdev->va_ccm_ccgrx);
	val &= ~(3<<led_cdev->clock_offset);
	val |= (3<<led_cdev->clock_offset);

	iowrite32(val,led_cdev->va_ccm_ccgrx);
	iowrite32(5,led_cdev->va_iomuxc_mux);
	iowrite32(0x1F838,led_cdev->va_iomux_pad);

	val = ioread32(led_cdev->va_gdir);
	val &= ~(1<<led_cdev->led_pin);
	val |= (1<<led_cdev->led_pin);

	iowrite32(val,led_cdev->va_gdir);

	val = ioread32(led_cdev->va_dr);
	val |= (0x01 << led_cdev->led_pin);
	iowrite32(val,led_cdev->va_dr);

	return 0;
}

static int led_chrdev_release(struct inode *inode,struct file *flip)
{
	struct drive_led *led_cdev = (struct drive_led *)container_of(inode->i_cdev,struct drive_led,dev);

	printk("release\n");

	iounmap(led_cdev->va_dr);
	iounmap(led_cdev->va_gdir);
	iounmap(led_cdev->va_iomuxc_mux);
	iounmap(led_cdev->va_ccm_ccgrx);
	iounmap(led_cdev->va_iomux_pad);
	return 0;
}

static ssize_t led_chrdev_write(struct file *filp,const char __user *buf, size_t count,loff_t * ppos)
{
	unsigned long val = 0;
	unsigned long ret = 0;

	int tmp = count;

	kstrtoul_from_user(buf,tmp,10,&ret);
	struct drive_led *led_cdev = (struct drive_led *)filp->private_data;

	val = ioread32(led_cdev->va_dr);
	if(ret == 0)
		val &= ~(0x01 << led_cdev->led_pin);
	else
		val |= (0x01 << led_cdev->led_pin);

	iowrite32(val,led_cdev->va_dr);
	*ppos += tmp;
	return tmp;
}

static struct file_operations led_op = 
{
	.owner = THIS_MODULE,
	.open = led_chrdev_open,
	.release = led_chrdev_release,
	.write = led_chrdev_write,
};

static struct drive_led drv_led[DEV_NUM] = 
{
	{.pa_dr = 0x0209C000,.pa_gdir = 0x0209C004,.pa_iomuxc_mux =
	0x20E006C,.pa_ccm_ccgrx = 0x20C406C,.pa_iomux_pad = 0x20E02F8,
	.led_pin = 4,.clock_offset = 26},
	{.pa_dr = 0x20A8000,.pa_gdir = 0x20A8004,.pa_iomuxc_mux =
	0x20E01E0,.pa_ccm_ccgrx = 0x20C4074,.pa_iomux_pad = 0x20E046C,
	.led_pin = 20,.clock_offset = 12},
	{.pa_dr = 0x20A8000,.pa_gdir = 0x20A8004,.pa_iomuxc_mux =
	0x20E01DC,.pa_ccm_ccgrx = 0x20C4074,.pa_iomux_pad = 0x20E0468,
	.led_pin = 19,.clock_offset = 12},
};

static int __init drv_led_init(void)
{
	dev_t cur;
	int i;

	printk("led chrdev init\n");

	alloc_chrdev_region(&devno,0,DEV_NUM,DEV_NAME);
	led_class = class_create(THIS_MODULE,"led_chrdev");

	for(i=0; i<DEV_NUM; i++)
	{
		cur = MKDEV(MAJOR(devno),MINOR(devno)+i);
		drv_led[i].dev.owner = THIS_MODULE;
		cdev_init(&drv_led[i].dev,&led_op);
		cdev_add(&drv_led[i].dev,cur,1);
		device_create(led_class,NULL,cur,NULL,DEV_NAME "%d",i);
	}
	return 0;
}
module_init(drv_led_init);

static void __exit drv_led_exit(void)
{
	dev_t cur;
	int i=0;

	printk("led chrdev exit\n");

	for(i=0; i<DEV_NUM; i++)
	{
		cur = MKDEV(MAJOR(devno),MINOR(devno)+i);
		device_destroy(led_class,cur);
		cdev_del(&drv_led[i].dev);
	}
	unregister_chrdev_region(devno,DEV_NUM);
	class_destroy(led_class);
}
module_exit(drv_led_exit);

MODULE_LICENSE("GPL");

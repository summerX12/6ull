#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

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
#include <linux/irq.h>
#include <linux/of_irq.h>

#define DEV_NAME    "button"
#define DEV_CNT     (1)

static dev_t button_devno;
struct class *class_button;
static struct cdev button_cdev;
struct device *device_button;


struct device_node	*button_device_node = NULL;  
unsigned  button_GPIO_number = 0;  
u32  interrupt_number = 0;         
atomic_t   button_status = ATOMIC_INIT(0);  

struct tasklet_struct button_tasklet;

void button_tasklet_handler(unsigned long data)
{
    int counter = 1;
    mdelay(200);
    printk(KERN_ERR "button_tasklet_hander counter = %d  \n", counter++);
    mdelay(200);
    printk(KERN_ERR "button_tasklet_hander counter = %d  \n", counter++);
    mdelay(200);
    printk(KERN_ERR "button_tasklet_hander counter = %d  \n", counter++);
    mdelay(200);
    printk(KERN_ERR "button_tasklet_hander counter = %d \n", counter++);
    mdelay(200);
    printk(KERN_ERR "button_tasklet_hander counter = %d \n", counter++);
}


static irqreturn_t button_irq_hander(int irq, void *dev_id)
{
    printk("handler irq inter\n");

	atomic_inc(&button_status);

    tasklet_schedule(&button_tasklet);

    printk("handler irq exit\n");

	return IRQ_RETVAL(IRQ_HANDLED);
}

static int button_open(struct inode *inode,struct file *filp)
{
    int err;

    button_device_node = of_find_node_by_path("/button_interrupt");
    if(NULL == button_device_node)
	{
		printk("of_find_node_by_path error!");
		return -1;
	}
    
    button_GPIO_number = of_get_named_gpio(button_device_node,"button_gpio",0);
 	if(0 == button_GPIO_number)
	{
		printk("of_get_named_gpio error");
		return -1;
	}  

    err = gpio_request(button_GPIO_number,"button_gpio");
    if(err < 0)
    {
		printk("gpio_request error");
		gpio_free(button_GPIO_number);
		return -1;
    }

    err = gpio_direction_input(button_GPIO_number);

    interrupt_number = irq_of_parse_and_map(button_device_node, 0);
    printk("\n irq_of_parse_and_map! =  %d \n",interrupt_number);

	err = request_irq(interrupt_number,button_irq_hander,IRQF_TRIGGER_RISING,"button_interrupt",device_button);
	if(err != 0)
	{
		printk("request_irq error");
		free_irq(interrupt_number, device_button);
		return -1;
	}

    tasklet_init(&button_tasklet,button_tasklet_handler,0);
	/*申请之后已经开启了，切记不要再次打开，否则运行时报错*/
	return 0;
}

static int button_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	int error = -1;
	int button_countervc = 0;

	button_countervc = atomic_read(&button_status);

	error = copy_to_user(buf, &button_countervc, sizeof(button_countervc));
	if(error < 0)
	{
		printk("copy_to_user error");
		return -1;
	}

	atomic_set(&button_status,0);
	return 0;
}

static int button_release(struct inode *inode,struct file *filp)
{
    gpio_free(button_GPIO_number);
    free_irq(interrupt_number,device_button);
    return 0;
}

static struct file_operations button_ops = 
{
    .owner = THIS_MODULE,
    .open = button_open,
    .read = button_read,
    .release = button_release,
};

static int __init button_driver_init(void)
{
    int err;

    err = alloc_chrdev_region(&button_devno,0,DEV_CNT,DEV_NAME);
    if(err < 0)
    {
        printk("fail to alloc button_devno\n");
        goto alloc_err;
    }
    button_cdev.owner = THIS_MODULE;
    cdev_init(&button_cdev,&button_ops);

    err = cdev_add(&button_cdev,button_devno,DEV_CNT);
    if(err < 0)
    {
        printk("fail to add\n");
        goto add_err;
    }

    class_button = class_create(THIS_MODULE,DEV_NAME);
    device_button = device_create(class_button,NULL,button_devno,NULL,DEV_NAME);

    return 0;

add_err:
    unregister_chrdev_region(button_devno,DEV_CNT);
    printk("error\n");
alloc_err:
    return -1;
}
module_init(button_driver_init);

static void __exit button_driver_exit(void)
{
    printk("exit\n");
    device_destroy(class_button,button_devno);
    class_destroy(class_button);
    cdev_del(&button_cdev);
    unregister_chrdev_region(button_devno,DEV_CNT);
}
module_exit(button_driver_exit);

MODULE_LICENSE("GPL");

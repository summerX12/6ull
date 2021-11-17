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

#include <linux/input.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>



struct device_node	*button_device_node = NULL;  
unsigned  button_GPIO_number = 0;  
u32  interrupt_number = 0;         

#define BUTTON_NAME "button"
struct input_dev *button_input_dev=NULL;


static irqreturn_t button_irq_hander(int irq, void *dev_id)
{
    int button_satus = 0;

	/*读取按键引脚的电平，根据读取得到的结果输入按键状态*/
	button_satus = gpio_get_value(button_GPIO_number);
	if(0 == button_satus)
	{
		input_report_key(button_input_dev, KEY_1, 0);
		input_sync(button_input_dev);
	}
	else
	{
		input_report_key(button_input_dev, KEY_1, 1);
		input_sync(button_input_dev);
	}
	return IRQ_HANDLED;
}

static int __init button_driver_init(void)
{
    int err;

    printk(KERN_ERR "button driver init\n");

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

	err = request_irq(interrupt_number,button_irq_hander,IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,"button_interrupt",NULL);
	if(err != 0)
	{
		printk("request_irq error");
        gpio_free(button_GPIO_number);
		free_irq(interrupt_number,NULL);
		return -1;
	}
	


    button_input_dev = input_allocate_device();
    if(NULL == button_input_dev)
    {
        printk(KERN_ERR "input allloc err\n");
        return -1;
    }

    button_input_dev->name = BUTTON_NAME;
    button_input_dev->evbit[0] = BIT_MASK(EV_KEY);
    input_set_capability(button_input_dev,EV_KEY,KEY_1);

    err = input_register_device(button_input_dev);
    if(err != 0)
    {
        printk(KERN_ERR "input_register_device err");
        gpio_free(button_GPIO_number);
        free_irq(interrupt_number,NULL);
        input_unregister_device(button_input_dev);
        return -1;
    }

    return 0;
}
module_init(button_driver_init);

static void __exit button_driver_exit(void)
{
    printk(KERN_ERR "button driver exit \n");
    gpio_free(button_GPIO_number);
    free_irq(interrupt_number,NULL);
    input_unregister_device(button_input_dev);
}
module_exit(button_driver_exit);

MODULE_LICENSE("GPL");

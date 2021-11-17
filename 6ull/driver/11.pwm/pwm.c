#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/err.h>
#include <linux/slab.h>


struct pwm_device	*red_led_pwm;

static int led_pwm_probe(struct platform_device *pdev)
{
    int ret = 0;
    struct device_node *child; // 保存子节点
    struct device *dev = &pdev->dev;
    printk("match success \n");

    /*--------------第一部分-----------------*/
    child = of_get_next_child(dev->of_node, NULL);
    if (child)
    {
            /*--------------第二部分-----------------*/
            red_led_pwm = devm_of_pwm_get(dev, child, NULL);
            if (IS_ERR(red_led_pwm))
            {
                    printk(KERN_ERR" red_led_pwm,get pwm  error!!\n");
                    return -1;
            }
    }
    else
    {
            printk(KERN_ERR" red_led_pwm of_get_next_child  error!!\n");
            return -1;
    }

    /*--------------第三部分-----------------*/
    pwm_config(red_led_pwm, 1000, 5000);
    pwm_set_polarity(red_led_pwm, PWM_POLARITY_INVERSED);
    pwm_enable(red_led_pwm);

    return ret;
}

static int led_pwm_remove(struct platform_device *pdev)
{
    pwm_config(red_led_pwm, 0, 5000);
    return 0;
}


static const struct of_device_id of_pwm_leds_match[] = 
{
    {.compatible = "red_led_pwm"},
    {},
};

struct platform_driver led_pwm_driver = 
{
    .probe = led_pwm_probe,
    .remove = led_pwm_remove,
    .driver = {
           .name = "led_pwm",
           .of_match_table = of_pwm_leds_match,
    },
};



static int __init led_pwm_init(void)
{
    printk(KERN_ERR "pwm init\n");

    return platform_driver_register(&led_pwm_driver);
}
module_init(led_pwm_init);

static void __exit led_pwm_exit(void)
{
    printk(KERN_ERR "pwm exit\n");
    
    platform_driver_unregister(&led_pwm_driver);
}
module_exit(led_pwm_exit);

MODULE_LICENSE("GPL");

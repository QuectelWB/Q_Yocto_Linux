#include <linux/types.h>
#include <linux/pm.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <asm/setup.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/stat.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>
 
int test_gpio0 = -1;
 
static struct class * test_gpios_class = NULL;
static struct device * test_gpio0_dev = NULL;
 
#define CTL_POWER_ON "1"
#define CTL_POWER_OFF "0"
 
//此函数会在使用 cat命令时候自动调用
static ssize_t test_gpio0_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	int gpio_state = gpio_get_value(test_gpio0);
	sprintf(buf, "%d\n", gpio_state);
	return strlen(buf);
}
 
//此函数会在使用 echo 命令时候自动调用
static ssize_t test_gpio0_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	if(!strncmp(buf, CTL_POWER_ON, strlen(CTL_POWER_ON))) {
		gpio_set_value(test_gpio0, 1);
	} else if(!strncmp(buf, CTL_POWER_OFF, strlen(CTL_POWER_OFF))) {
		gpio_set_value(test_gpio0, 0);
	}
	return count;
}
 
static struct device_attribute test_gpio0_dev_attr = {
	.attr = {
		.name = "state",
		.mode = S_IRWXU|S_IRWXG|S_IRWXO,
	},
	.show = test_gpio0_show,
	.store = test_gpio0_store,
};
 
 
static int test_gpio_probe(struct platform_device *pdev)
{
	int ret = 0;
	printk("zc enter test_gpio_probe \n");
	//寻找设备树dts下的 "test_gpio,gpio0" 名称对应的GPIO口
	test_gpio0 = of_get_named_gpio(pdev->dev.of_node, "test_gpio,gpio0", 0);
	if (test_gpio0 < 0)
		printk("test_gpio0 is not available \n");
	//对应修改的dts的 label
	ret = gpio_request(test_gpio0, "test_gpio");
	if(0 != ret) {
		printk("gpio request %d failed.", test_gpio0);
		goto fail1;
	}
	
	//设置gpio为输出
	gpio_direction_output(test_gpio0, 0);
	
	//创建test_gpios节点
	test_gpios_class = class_create(THIS_MODULE, "test_gpios");
	if(IS_ERR(test_gpios_class))
	{
		ret = PTR_ERR(test_gpios_class);
		printk("Failed to create class.\n");
		return ret;
	}
	
	//创建test_gpio0 的驱动设备
	test_gpio0_dev=device_create(test_gpios_class, NULL, 0, NULL, "test_gpio0");
	if (IS_ERR(test_gpio0_dev))
	{
		ret = PTR_ERR(test_gpios_class);
		printk("Failed to create device(test_gpio0_dev)!\n");
		return ret;
	}
	
	//根据 test_gpio0_dev_attr 配置，生成相应的文件
	ret = device_create_file(test_gpio0_dev, & test_gpio0_dev_attr);
	if(ret)
	{
		pr_err("%s: test_gpio0 creat sysfs failed\n",__func__);
		return ret;
	}
	printk("enter test_gpio_probe, ok \n");
fail1:
	return ret;
}
 
 
static int test_gpio_remove(struct platform_device *pdev)
{
	device_destroy(test_gpios_class, 0);
	class_destroy(test_gpios_class);
	device_remove_file(test_gpio0_dev, & test_gpio0_dev_attr);
	return 0;
}
static int test_gpio_suspend(struct platform_device *pdev,pm_message_t state)
{
	return 0;
}
 
static int test_gpio_resume(struct platform_device *pdev)
{
	return 0;
}
 
 
static struct of_device_id test_gpio_dt_match[] = {
	{ .compatible = "test_gpio",},
	{ },
};
 
 
MODULE_DEVICE_TABLE(of, test_gpio_dt_match);
 
//驱动的方法与函数对应
static struct platform_driver gpio_power_driver = {
	.driver = {
		.name = "test_gpio",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(test_gpio_dt_match),
	},
	.probe = test_gpio_probe,
	.remove = test_gpio_remove,
	.suspend = test_gpio_suspend,
	.resume = test_gpio_resume,
};
 
 
static __init int gpio_power_init(void)
{
	return platform_driver_register(&gpio_power_driver);
}
static void __exit gpio_power_exit(void)
{
	platform_driver_unregister(&gpio_power_driver);
}
 
 
module_init(gpio_power_init);
module_exit(gpio_power_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test_gpio Driver");

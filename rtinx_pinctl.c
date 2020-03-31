/*
********************************************************************************
** File:
**        pinctl.c
**
** Description:
**        Pin control driver.
********************************************************************************
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#define	RTINX_IOCTL_MAGIC      'x'
#define	RTINX_IOCTL_GET_PIN    _IOR(RTINX_IOCTL_MAGIC, 1, unsigned int)
#define	RTINX_IOCTL_SET_PIN    _IOW(RTINX_IOCTL_MAGIC, 2, unsigned int)

struct pinctl
{
    int index;
    int type;
    int value;
};

static long pinctl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;

    int ret = 0;
    unsigned int port = 0, pin = 0, value = 0, command = 0;

    if (get_user(command, p))
        return -EFAULT;

    port = (command >> 16) & 0xff;
    pin = (command >> 8) & 0xff;
    value = command & 0xff;
	
    switch (cmd) 
    {
    case RTINX_IOCTL_SET_PIN:
		printk("RTINX_IOCTL_SET_PIN: port=%d, pin=%d, value=%d\n", port, pin, value);
#if defined(IMX)
        gpio_set_value(IMX_GPIO_NR(port, pin), value);
#endif
        break;
    case RTINX_IOCTL_GET_PIN:
#if defined(IMX)
        value = gpio_get_value(IMX_GPIO_NR(port, pin));
#endif
        printk("RTINX_IOCTL_GET_PIN: port=%d, pin=%d, value=%d\n", port, pin, value);
        ret = put_user(value, p);
        break;
    }
    
    return ret;
}

static struct file_operations pinctl_fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = pinctl_ioctl
};

static struct miscdevice pinctl_miscdev = 
{
    .minor = MISC_DYNAMIC_MINOR, 
    .name = "pinctl",             
    .fops = &pinctl_fops 
};

static int pinctl_init(void)
{
    misc_register(&pinctl_miscdev);    
	
    printk("%s\n", __func__);

    return 0;
}

static void pinctl_exit(void)
{
    printk("%s\n", __func__);    
    misc_deregister(&pinctl_miscdev);
}

module_init(pinctl_init);
module_exit(pinctl_exit);
MODULE_LICENSE("GPL");

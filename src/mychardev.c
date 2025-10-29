#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // for copy_to_user, copy_from_user
#include <linux/cdev.h>
#include <linux/device.h>
#include "mychardev.h"


int major;
char kernel_buffer[BUF_LEN];
struct class *mychardev_class;
struct cdev my_cdev;

static int mychardev_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "mychardev: Device opened\n");
	return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "mychardev: Device closed\n");
	return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	int bytes_read = 0;
	if(*offset >= BUF_LEN)
		return 0;

	if(len > BUF_LEN - *offset)
		len = BUF_LEN - *offset;
	if (copy_to_user(buf, kernel_buffer + *offset, len))
		return -EFAULT;

	*offset += len;
	bytes_read = len;
	printk(KERN_INFO "mychardev: Sent %d bytes to user \n", bytes_read);
	return bytes_read;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t len,loff_t *offset)
{
	if(len > BUF_LEN)
		len = BUF_LEN;

	if(copy_from_user(kernel_buffer, buf, len))
		return -EFAULT;

	kernel_buffer[len] = '\0';
	printk(KERN_INFO "mychardev: Received %zu bytes: %s\n",len, kernel_buffer);
	return len;
}

//File operations structure
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = mychardev_open,
	.release = mychardev_release,
	.read = mychardev_read,
	.write = mychardev_write,
};

static int __init mychardev_init(void)
{
	dev_t dev;
	//Allocate a Device number dynamically

	if(alloc_chrdev_region(&dev, 0 ,1, DEVICE_NAME) < 0) {
		printk(KERN_ALERT "mychardev: Failed to allocate major number \n");
		return -1;
	}

	major = MAJOR(dev);
	printk(KERN_INFO "mychardev: Registered with major number %d\n", major);

	//Create device class
	mychardev_class = class_create(DEVICE_NAME);
	if(IS_ERR(mychardev_class)){
		unregister_chrdev_region(dev,1);
		return PTR_ERR(mychardev_class);
	}


	//Create device node in /dev
	if(IS_ERR(device_create(mychardev_class, NULL,dev, NULL, DEVICE_NAME))) {
		class_destroy(mychardev_class);
		unregister_chrdev_region(dev, 1);
		return -1;
	}


	//Initialize and add cdev
	cdev_init(&my_cdev, &fops);
	if( cdev_add(&my_cdev, dev, 1) < 0) {
		device_destroy(mychardev_class, dev);
		class_destroy(mychardev_class);
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	printk(KERN_INFO "mychardev: Module loaded successfully\n");
	return 0;
}


static void __exit mychardev_exit(void)
{
	dev_t dev = MKDEV(major, 0);
	cdev_del(&my_cdev);
	device_destroy(mychardev_class, dev);
	class_destroy(mychardev_class);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "mychardev: Module unloaded\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shravya Panchagiri");
MODULE_DESCRIPTION("Basic Character Device Driver");


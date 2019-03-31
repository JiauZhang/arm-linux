#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

int init_hello(void)
{
	printk(KERN_EMERG "printk level: %s hello world!\n",KERN_EMERG);
	printk(KERN_ALERT "printk level: %s hello world!\n",KERN_ALERT);
	
	return 0;
}

void exit_hello(void)
{
	printk(KERN_EMERG "printk level: %s goodbye world!\n",KERN_EMERG);
	printk(KERN_ALERT "printk level: %s goodbye world!\n",KERN_ALERT);
	
//	return 0;
}

module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINUX");
MODULE_DESCRIPTION("A simple module");

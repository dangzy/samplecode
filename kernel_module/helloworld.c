#include <linux/init.h>
#include <linux/kernel.h>  
#include <linux/module.h>

static int hello_init(void)
{ 
    printk(KERN_CRIT "Hello! This is the helloworld module!\n");
    return 0;
} 

static void hello_exit(void)
{
    printk(KERN_CRIT "Module exit! Bye Bye!\n");
    return;
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

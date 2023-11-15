#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static char *name = "world";
module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "Chose à saluer");

static __init int hello_init(void)
{
	printk(KERN_ALERT "hello, %s\n", name);
	return 0;
}

static __exit void hello_cleanup(void)
{
	printk(KERN_ALERT "goodbye, %s\n", name);
}

module_init(hello_init);
module_exit(hello_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alban Gruin");
MODULE_DESCRIPTION("hello, world");

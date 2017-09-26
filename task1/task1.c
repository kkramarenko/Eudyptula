/*
 * * This Linux kernel module prints "Hello World!" in to kernel debug log   
 * * level when loading. 
 * */

#include <linux/init.h>
#include <linux/module.h>

static int __init task1_init(void)
{
  printk(KERN_DEBUG "Hello World!\n");
  return 0;
}

static void __exit task1_exit(void)
{
}

module_init(task1_init);
module_exit(task1_exit);
MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION("A sample linux kernel module");
MODULE_AUTHOR("Konstantin Kramarenko <kostya.kram@gmail.com>");


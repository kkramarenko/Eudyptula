#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUF_MAX 13

static char *task6_string = "0a0d07c2a690\0";

static ssize_t misc_read(struct file *fp, char __user *buff, size_t size,
			loff_t *offset)
{
	return simple_read_from_buffer(buff, size, offset, task6_string, 
					BUF_MAX);
}

static int misc_write(struct file *fp, const char __user *buff, size_t size,
			loff_t *offset)
{
	char written_string[BUF_MAX];
	size_t rc = 0;

	rc = simple_write_to_buffer(written_string, BUF_MAX, offset, buff, size);
	if (rc < 0)
		return rc;

	written_string[BUF_MAX - 1] = '\0';
	if (strcmp(task6_string, written_string) != 0)
		return -EINVAL;
	return rc;
}
static const struct file_operations misc_fops = {
	.read = misc_read,
	.write = misc_write
};

static struct miscdevice misc_dev_st = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &misc_fops,
	.mode = 0666
};

static int __init task6_init(void)
{
	int res = 0;

	res = misc_register(&misc_dev_st);
	if (res)
		pr_err("Uneable to register misc device!\n");
	return res;
}

static void __exit task6_exit(void)
{
	misc_deregister(&misc_dev_st);
}

module_init(task6_init);
module_exit(task6_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simpe module for misc device");
MODULE_AUTHOR("Konstantin Kramarenko <kostya.kram@gmail.com>");

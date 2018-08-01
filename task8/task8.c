#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/debugfs.h>

#define BUF_MAX 13

static char *task8_string = "0a0d07c2a690\0";

static struct dentry *dent, *fent;

static ssize_t dfs_read_id(struct file *fp, char __user *buff, size_t size,
			loff_t *offset)
{
	return simple_read_from_buffer(buff, size, offset, task8_string, 
					BUF_MAX);
}

static ssize_t dfs_write_id(struct file *fp, const char __user *buff, size_t size,
			loff_t *offset)
{
	char written_string[BUF_MAX];
	size_t rc = 0;

	rc = simple_write_to_buffer(written_string, BUF_MAX, offset, buff, size);
	if (rc < 0)
		return rc;

	written_string[BUF_MAX - 1] = '\0';
	if (strcmp(task8_string, written_string) != 0)
		return -EINVAL;
	return rc;
}

static const struct file_operations fops_id = {
	.read = dfs_read_id,
	.write = dfs_write_id,
};

static int __init task8_init(void)
{
	dent = debugfs_create_dir("eudyptula", NULL);
	if (!dent) {
  		pr_info("TASK8 Creating directory ends with error!\n");
		return -1;
  	}
	fent = debugfs_create_file("id", 0666, dent, task8_string, &fops_id);
	if (!fent) {
		pr_info("TASK8 Creating file ends with error!\n");
		return -1;
	}

  return 0;
}

static void __exit task8_exit(void)
{
	debugfs_remove_recursive(dent);
}

module_init(task8_init);
module_exit(task8_exit);
MODULE_LICENSE ("GPL");
MODULE_DESCRIPTION("A sample linux kernel module with debugfs");
MODULE_AUTHOR("Konstantin Kramarenko <kostya.kram@gmail.com>");


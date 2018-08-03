#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/rwlock.h>
#include <linux/rwlock_types.h>
#include <linux/slab.h>

#define BUF_MAX 13

static rwlock_t rwlock;
static char *task8_foo;
static char *task8_string = "0a0d07c2a690\0";
static unsigned long long int *task8_jiffies =
				(unsigned long long int *) &jiffies;

static struct dentry *dent, *fent, *u64ent, *fooent;

static ssize_t dfs_read_id(struct file *fp, char __user *buff, size_t size,
		loff_t *offset)
{
	return simple_read_from_buffer(buff, size, offset, task8_string,
					BUF_MAX);
}

static ssize_t dfs_read_foo(struct file *fp, char __user *buff, size_t size,
		loff_t *offset)
{
	size_t rc = 0;

	read_lock(&rwlock);
	rc = simple_read_from_buffer(buff, size, offset, task8_foo,
					PAGE_SIZE);
	read_unlock(&rwlock);

	return rc;
}

static ssize_t dfs_write_foo(struct file *fp, const char __user *buff,
		size_t size, loff_t *offset)
{
	size_t rc = 0;

	if (size > PAGE_SIZE)
		return -EINVAL;

	write_lock(&rwlock);
	rc = simple_write_to_buffer(task8_foo, PAGE_SIZE, offset, buff,
					size);
	write_unlock(&rwlock);

	return rc;
}

static ssize_t dfs_write_id(struct file *fp, const char __user *buff,
		size_t size, loff_t *offset)
{
	char written_string[BUF_MAX];
	size_t rc = 0;

	rc = simple_write_to_buffer(written_string, BUF_MAX, offset, buff,
					size);
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

static const struct file_operations fops_foo = {
	.read = dfs_read_foo,
	.write = dfs_write_foo,
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

	u64ent = debugfs_create_u64("jiffies", 0444, dent, task8_jiffies);
	if (!u64ent) {
		pr_info("TASK8 Creating jiffies file ends with error!\n");
		return -1;
	}

	task8_foo = kmalloc(PAGE_SIZE, GFP_KERNEL);
	rwlock_init(&rwlock);
	fooent = debugfs_create_file("foo", 0644, dent, task8_foo, &fops_foo);
	if (!fooent) {
		pr_info("TASK8 Creating foo file ends with error!\n");
		return -1;
	}

	return 0;
}

static void __exit task8_exit(void)
{
	kfree(task8_foo);
	debugfs_remove_recursive(dent);
}

module_init(task8_init);
module_exit(task8_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A sample linux kernel module with debugfs");
MODULE_AUTHOR("Konstantin Kramarenko <kostya.kram@gmail.com>");


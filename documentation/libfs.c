/*
 * Demonstrate a trivial filesystem using libfs.  This is the extra
 * simple version without subdirectories.
 *
 * Copyright 2002, 2003 Jonathan Corbet <corbet-AT-lwn.net>
 * This file may be redistributed under the terms of the GNU GPL.
 *
 * Chances are that this code will crash your system, delete your
 * nethack high scores, and set your disk drives on fire.  You have
 * been warned.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> /* This is where libfs stuff is declared */
#include <asm/atomic.h>
#include <asm/uaccess.h> /* copy_to_user */

/*
 * Boilerplate stuff.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonathan Corbet");

#define LFS_MAGIC 0x19980122

/*
 * Implement an array of counters.
 */
#define NCOUNTERS 4
static atomic_t counters[NCOUNTERS];

/*
 * The operations on our "files".
 */

/*
 * Open a file.  All we have to do here is to copy over a
 * copy of the counter pointer so it's easier to get at.
 */
static int lfs_open(struct inode *inode, struct file *filp)
{
	if (inode->i_ino > NCOUNTERS)
		return -ENODEV; /* Should never happen.  */
	filp->private_data = counters + inode->i_ino - 1;
	return 0;
}

#define TMPSIZE 20
/*
 * Read a file.  Here we increment and read the counter, then pass it
 * back to the caller.  The increment only happens if the read is done
 * at the beginning of the file (offset = 0); otherwise we end up counting
 * by twos.
 */
static ssize_t lfs_read_file(struct file *filp, char *buf, size_t count,
			     loff_t *offset)
{
	int v, len;
	char tmp[TMPSIZE];
	atomic_t *counter = (atomic_t *)filp->private_data;
	/*
 * Encode the value, and figure out how much of it we can pass back.
 */
	v = atomic_read(counter);
	if (*offset > 0)
		v -= 1; /* the value returned when offset was zero */
	else
		atomic_inc(counter);
	len = snprintf(tmp, TMPSIZE, "%d\n", v);
	if (*offset > len)
		return 0;
	if (count > len - *offset)
		count = len - *offset;
	/*
 * Copy it back, increment the offset, and we're done.
 */
	if (copy_to_user(buf, tmp + *offset, count))
		return -EFAULT;
	*offset += count;
	return count;
}

/*
 * Write a file.
 */
static ssize_t lfs_write_file(struct file *filp, const char *buf, size_t count,
			      loff_t *offset)
{
	char tmp[TMPSIZE];
	atomic_t *counter = (atomic_t *)filp->private_data;
	/*
 * Only write from the beginning.
 */
	if (*offset != 0)
		return -EINVAL;
	/*
 * Read the value from the user.
 */
	if (count >= TMPSIZE)
		return -EINVAL;
	memset(tmp, 0, TMPSIZE);
	if (copy_from_user(tmp, buf, count))
		return -EFAULT;
	/*
 * Store it in the counter and we are done.
 */
	atomic_set(counter, simple_strtol(tmp, NULL, 10));
	return count;
}

/*
 * Now we can put together our file operations structure.
 */
static struct file_operations lfs_file_ops = {
	.open = lfs_open,
	.read = lfs_read_file,
	.write = lfs_write_file,
};

/*
 * OK, create the files that we export.
 */
struct tree_descr OurFiles[] = {
	{ NULL, NULL, 0 }, /* Skipped */
	{ .name = "counter0", .ops = &lfs_file_ops, .mode = S_IWUSR | S_IRUGO },
	{ .name = "counter1", .ops = &lfs_file_ops, .mode = S_IWUSR | S_IRUGO },
	{ .name = "counter2", .ops = &lfs_file_ops, .mode = S_IWUSR | S_IRUGO },
	{ .name = "counter3", .ops = &lfs_file_ops, .mode = S_IWUSR | S_IRUGO },
	{ "", NULL, 0 }
};

/*
 * Superblock stuff.  This is all boilerplate to give the vfs something
 * that looks like a filesystem to work with.
 */

/*
 * "Fill" a superblock with mundane stuff.
 */
static int lfs_fill_super(struct super_block *sb, void *data, int silent)
{
	return simple_fill_super(sb, LFS_MAGIC, OurFiles);
}

/*
 * Stuff to pass in when registering the filesystem.
 */
static struct super_block *lfs_get_super(struct file_system_type *fst,
					 int flags, const char *devname,
					 void *data)
{
	return get_sb_single(fst, flags, data, lfs_fill_super);
}

static struct file_system_type lfs_type = {
	.owner = THIS_MODULE,
	.name = "lwnfs",
	.get_sb = lfs_get_super,
	.kill_sb = kill_litter_super,
};

/*
 * Get things set up.
 */
static int __init lfs_init(void)
{
	int i;

	for (i = 0; i < NCOUNTERS; i++)
		atomic_set(counters + i, 0);
	return register_filesystem(&lfs_type);
}

static void __exit lfs_exit(void)
{
	unregister_filesystem(&lfs_type);
}

module_init(lfs_init);
module_exit(lfs_exit);

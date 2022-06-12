#include "fs_mount.h"
#include "fs_info.h"

#define CRYPTFS_MAGIC 0xDEADC0DE
#define CRYPTFS_BLOCKSIZE 1024
#define CRYPTFS_BLOCKSIZE_BITS 10
#define CRYPTFS_TIME_GRAN 1

/**
 * @brief Filesystem initial definitions
 */
struct file_system_type fs_type = {
	.name = CRYPT_FS_NAME, // The name of the filesystem
	.mount =
		mount_cryptfs, // The function which will be called when the filesystem is mounted
	.kill_sb =
		unmount_cryptfs, // The function which will be called when the filesystem is killed (unmounted)
	.owner = THIS_MODULE, // The owner of the filesystem
	.fs_flags = 0 // The flags of the filesystem
};

/**
 * @brief All the "file/directory" operations
 */
static struct super_operations const cryptfs_super_ops = {
	.alloc_inode =
		cryptfs_alloc_inode, // The function which will be called when a new inode is created
	.drop_inode =
		cryptfs_drop_inode, // The function which will be called when an inode is destroyed
	.put_super =
		cryptfs_put_super // The function which will be called at unmount => to destroy the superblock
};

struct dentry *mount_cryptfs(struct file_system_type *fs_type, int flags,
			     const char *dev_name, void *data)
{
	pr_debug(CRYPT_FS_NAME ": Mounting...\n");

	struct dentry *dentry = mount_nodev(fs_type, flags, data, fs_fill_sb);
	if (IS_ERR(dentry)) {
		pr_err(CRYPT_FS_NAME ": cannot mount filesystem\n");
		return dentry;
	}

	pr_debug(CRYPT_FS_NAME ": Mounted successfully...\n");
	return dentry;
}

int fs_fill_sb(struct super_block *sb, void *data, int silent)
{
	pr_debug(CRYPT_FS_NAME ": Filling superblock...\n");

	// The cipher key
	sb->s_fs_info = (void *)data;
	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = CRYPTFS_BLOCKSIZE;
	sb->s_blocksize_bits = CRYPTFS_BLOCKSIZE_BITS;
	sb->s_magic = CRYPTFS_MAGIC;
	sb->s_op = &cryptfs_super_ops;
	sb->s_time_gran = CRYPTFS_TIME_GRAN;
	sb->s_flags = 0;

	// TODO : Check if the key is valid (on the image)
	if (cryptfs_check_key(sb->s_fs_info) != 0) {
		pr_err(CRYPT_FS_NAME ": Invalid key\n");
		return -EINVAL;
	}

	pr_debug(CRYPT_FS_NAME ": Filled superblock successfully...\n");
	return 0;
}

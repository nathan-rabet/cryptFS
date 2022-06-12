#ifndef FS_MOUNT_H
#define FS_MOUNT_H

struct file_system_type;

#include <linux/fs.h>

struct dentry *mount_cryptfs(struct file_system_type *fs_type, int flags,
                             const char *dev_name, void *data);

void unmount_cryptfs(struct super_block *sb);


struct inode *cryptfs_alloc_inode(struct super_block *sb);

int cryptfs_drop_inode(struct inode *node);

void cryptfs_put_super(struct super_block *sb);


int fs_fill_sb(struct super_block *sb, void *data, int silent);

#endif /* FS_MOUNT_H */

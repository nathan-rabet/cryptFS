#ifndef FS_MOUNT_H
#define FS_MOUNT_H

#include <linux/fs.h>

struct dentry *mount_cryptfs(struct file_system_type *fs_type, int flags,
			     const char *dev_name, void *data);

void *unmount_cryptfs(struct super_block *sb);

#endif /* FS_MOUNT_H */

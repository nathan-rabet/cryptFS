#ifndef FS_INFO_H
#define FS_INFO_H

#include "fs_mount.h"

#include <linux/fs.h>

#define CRYPT_FS_NAME "cryptfs"

extern struct file_system_type fs_type;

#endif /* FS_INFO_H */

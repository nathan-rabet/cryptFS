#include "cryptofuse.h"

#include <errno.h>

void *cryptfs_init(struct fuse_conn_info *info)
{
    printf("Using FUSE protocol %d.%d\n", info->proto_major, info->proto_minor);
    //TODO allocate data

    return NULL;
}

int cryptfs_open(const char *path, struct fuse_file_info *file)
{
    puts("open");
    if((file->flags & 3) != O_RDONLY)
        return -EACCES;

    file->fh = inode_get_idx_by_path(path);
    printf("%s is inode %d\n", path, file->fh);

    return 0;
}
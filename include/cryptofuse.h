#ifndef CRYPTOFUSE_H

#define FUSE_USE_VERSION 24

#include <fuse/fuse.h>

void *cryptfs_init(struct fuse_conn_info *info);
void cryptfs_destroy(void *ptr);
int cryptfs_statfs(const char *path, struct statvfs *stats);

int cryptfs_mkdir(const char *path, mode_t mode);
int cryptfs_mknod(const char *path, mode_t mode, dev_t num);

int cryptfs_rmdir(const char *path);

int cryptfs_getattr(const char *path, struct stat *st);
int cryptfs_open(const char *path, struct fuse_file_info *file);
int cryptfs_read(const char *path, char *buf, size_t sz, off_t offset,
            struct fuse_file_info *file);
int cryptfs_write(const char *path, const char *buf, size_t sz, off_t offset,
            struct fuse_file_info *file);
int cryptfs_write_buf(const char *path, struct fuse_bufvec *buf, off_t off,
            struct fuse_file_info *file);
int cryptfs_read_buf(const char *path, struct fuse_bufvec **bufp,
            size_t size, off_t off, struct fuse_file_info *file);
int cryptfs_opendir(const char *path, struct fuse_file_info *file);
int cryptfs_readdir(const char *path, void *data, fuse_fill_dir_t callback, off_t offset,
        struct fuse_file_info *file);
int cryptfs_release(const char *path, struct fuse_file_info *file);
int cryptfs_releasedir(const char *path, struct fuse_file_info *file);
int cryptfs_create(const char *path, mode_t mode, struct fuse_file_info *file);
int cryptfs_ftruncate(const char *path, off_t offset, struct fuse_file_info *file);

int cryptfs_flush(const char *path, struct fuse_file_info *file);
int cryptfs_fsync(const char *path, int, struct fuse_file_info *file);
int cryptfs_fsyncdir(const char *path, int, struct fuse_file_info *file);

int cryptfs_chmod(const char *path, mode_t mod);
int cryptfs_chown(const char *path, uid_t uid, gid_t gid);
int cryptfs_rename(const char *path, const char *name);

int cryptfs_fallocate(const char *path, int, off_t, off_t,
            struct fuse_file_info *file);

static struct fuse_operations cryptfs_ops = {
    .init = cryptfs_init,
    .open = cryptfs_open,
    .read = cryptfs_read
};

#endif /* CRYPTOFUSE_H */
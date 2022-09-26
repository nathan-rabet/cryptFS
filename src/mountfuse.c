#define FUSE_USE_VERSION 30

#include <errno.h>
#include <fcntl.h>
#include <fuse/fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "print.h"

char dir_list[256][256];
int dir_list_count = -1;

char files_list[256][256];
int files_list_count = -1;

char files_content[256][256];
int files_content_count = -1;

void add_dir(const char *dir_name)
{
    dir_list_count++;
    strcpy(dir_list[dir_list_count], dir_name);
}

int is_dir(const char *dir_name)
{
    dir_name++;
    int i;
    for (i = 0; i <= dir_list_count; i++)
    {
        if (strcmp(dir_list[i], dir_name) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int is_file(const char *path)
{
    for (int i = 0; i <= files_list_count; i++)
        if (strcmp(path, files_list[i]) == 0)
            return 1;

    return 0;
}
void add_file(const char *file_name)
{
    files_list_count++;
    strcpy(files_list[files_list_count], file_name);
    files_content_count++;
    strcpy(files_content[files_content_count], "hello");
}

int get_file_index(const char *file_name)
{
    file_name++;
    int i;
    for (i = 0; i <= files_list_count; i++)
    {
        if (strcmp(files_list[i], file_name) == 0)
        {
            return i;
        }
    }
    return -1;
}
void write_to_file(const char *file_name, const char *buf, size_t size,
                   off_t offset)
{
    int index = get_file_index(file_name);
    if (index == -1)
    {
        return;
    }

    strcpy(files_content[index] + offset, buf);
}

void *cryptfs_init(struct fuse_conn_info *info)
{
    print_info("init called !\n");
}

void cryptfs_destroy(void *ptr)
{
    print_info("destroy called !\n");
}

int cryptfs_statfs(const char *path, struct statvfs *stats)
{
    print_info("statfs called path : %s\n", path);

#ifdef test
    stats->f_bsize = get_block_size();
#endif
}

int cryptfs_getattr(const char *path, struct stat *st)
{
    print_info("getattr called path : %s\n", path);
    st->st_uid = getuid(); // The owner of the file/directory is the user who
                           // mounted the filesystem
    st->st_gid = getgid(); // The group of the file/directory is the same as the
                           // group of the user who mounted the filesystem
    st->st_atime =
        time(NULL); // The last "a"ccess of the file/directory is right now
    st->st_mtime = time(
        NULL); // The last "m"odification of the file/directory is right now

    if (strcmp(path, "/") == 0)
    {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is
                          // here: http://unix.stackexchange.com/a/101536
        return 0;
    }
    else if (is_file(++path) == 1)
    {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }
    else if (is_dir(path) == 1)
    {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 1;
    }
    else
        return -ENOENT;

    return 0;
}
int cryptfs_readdir(const char *path, void *data, fuse_fill_dir_t callback,
                    off_t offset, struct fuse_file_info *file)
{
    print_info("readdir called path : %s\n", path);
    callback(data, ".", NULL, 0);
    callback(data, "..", NULL, 0);

    if (strcmp(path, "/") == 0) // if we want to list the root directory
    {
        for (int i = 0; i <= dir_list_count; i++)
        {
            callback(data, dir_list[i], NULL, 0);
        }
        for (int i = 0; i <= files_list_count; i++)
            callback(data, files_list[i], NULL, 0);
        return 0;
    }
    return -EBADF;
}
int cryptfs_read(const char *path, char *buf, size_t sz, off_t offset,
                 struct fuse_file_info *file)
{
    print_info("read called path : %s\n", path);
    int index = get_file_index(path);

    if (is_dir(path))
        return -EISDIR;

    if (index == -1)
        return -EBADF;
    char *content = files_content[index];
    // TODO calculate real file length
    long max = 1024;
    if (offset + sz > max)
    {
        memcpy(buf, content + offset, max - offset);
        return max - offset;
    }
    else
        memcpy(buf, content + offset, sz);

    long r = strlen(content) - offset;
    if (r > sz)
        return sz;
    return r;
}

int cryptfs_write(const char *path, const char *buf, size_t sz, off_t offset,
                  struct fuse_file_info *file)
{
    print_info("write called path : %s\n", path);
    write_to_file(path, buf, sz, offset);
    return sz;
}

int cryptfs_mkdir(const char *path, mode_t mode)
{
    print_info("mkdir called path : %s\n", path);
    path++;
    add_dir(path);

    return 0;
}
int cryptfs_rmdir(const char *path)
{
    print_info("rmdir called path : %s\n", path);
    return -ENODEV;
}
int cryptfs_mknod(const char *path, mode_t mode, dev_t num)
{
    print_info("mknod called path : %s\n", path);
    path++;
    add_file(path);

    return 0;
}
int cryptfs_open(const char *path, struct fuse_file_info *file)
{
    print_info("open called path : %s\n", path);
    return 0;
}
int cryptfs_release(const char *path, struct fuse_file_info *file)
{
    print_info("release called path : %s\n", path);
    return 0;
}

int cryptfs_access(const char *path, int mode)
{
    print_info("access called path : %s\n", path);
    return 0;
}

int cryptfs_create(const char *path, mode_t mode, struct fuse_file_info *file)
{
    return 0;
}

static struct fuse_operations cryptfs_ops = { .init = cryptfs_init,
                                              .statfs = cryptfs_statfs,
                                              .destroy = cryptfs_destroy,
                                              .getattr = cryptfs_getattr,
                                              .readdir = cryptfs_readdir,
                                              .read = cryptfs_read,
                                              .write = cryptfs_write,
                                              .mkdir = cryptfs_mkdir,
                                              .mknod = cryptfs_mknod,
                                              .rmdir = cryptfs_rmdir,
                                              .open = cryptfs_open,
                                              .release = cryptfs_release,
                                              .access = cryptfs_access };
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        print_info("Usage: %s <mountpoint> <key>\n", argv[0]);
    }
    return fuse_main(argc, argv, &cryptfs_ops, NULL);
}

// run with: gcc -Wall mountfuse.c `pkg-config fuse --cflags --libs` -o
// mountfuse

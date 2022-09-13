#define FUSE_USE_VERSION 30

#include <fuse/fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static const char * test = "Hello World!";

int cryptfs_getattr(const char *path, struct stat *st)
{ 
    st->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
	st->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
	st->st_atime = time( NULL ); // The last "a"ccess of the file/directory is right now
	st->st_mtime = time( NULL ); // The last "m"odification of the file/directory is right now
	
    if ( strcmp( path, "/" ) == 0 )
	{
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	}
	else
	{
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	}
	
	return 0;
}
int cryptfs_readdir(const char *path, void *data, fuse_fill_dir_t callback, off_t offset,
        struct fuse_file_info *file)
{
    callback(data, ".", NULL, 0);
    callback(data, "..", NULL, 0);
    callback(data, "file.txt", NULL, 0);
    return 0;
}
int cryptfs_open(const char *path, struct fuse_file_info *file)
{
    if(strcmp(path, test) != 0)
        return -ENOENT;

    if((file->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}
int cryptfs_read(const char *path, char *buf, size_t sz, off_t offset,
            struct fuse_file_info *file)
{
    size_t len;
    (void) file;
    if(strcmp(path, test) != 0)
        return -ENOENT;

    len = strlen(test);
    if (offset < len) {
        if (offset + sz > len)
            sz = len - offset;
        memcpy(buf, test + offset, sz);
    } else
        sz = 0;

    return sz;
}
static struct fuse_operations cryptfs_ops = {
    .getattr = cryptfs_getattr,
    .readdir = cryptfs_readdir,
    .open = cryptfs_open,
    .read = cryptfs_read
};
int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <mountpoint> <key>", argv[0]);
    }
    return fuse_main(argc, argv, &cryptfs_ops, NULL);
}

//run with: gcc -Wall mountfuse.c `pkg-config fuse --cflags --libs` -o mountfuse

#define FUSE_USE_VERSION 30

#include <errno.h>
#include <fcntl.h>
#include <fuse/fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

char dir_list[1024][1024];
int dir_list_count = -1;

char files_list[1024][1024];
int files_list_count = -1;

char files_content[1024][1024];
int files_content_count = -1;

void add_dir(char *dir_name)
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

void add_file(const char *file_name)
{
    files_list_count++;
    strcpy(files_list[files_list_count], file_name);
    files_content_count++;
    strcpy(files_content[files_content_count], "");
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
void write_to_file(const char *file_name, const char *buf, size_t size, off_t offset)
{
    int index = get_file_index(file_name);
    if (index == -1)
    {
        return;
    }
    int i;
    for (i = 0; i < size; i++)
    {
        files_content[index][offset + i] = buf[i];
    }
}


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
    
    if ( strcmp(path, "/") == 0) // if we want to list the root directory
    {
        for (int i = 0; i <= dir_list_count; i++)
            callback(data, dir_list[i], NULL,0);
         for (int i = 0; i <= files_list_count; i++)
            callback(data, files_list[i], NULL,0);
    }
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
        printf("Usage: %s <mountpoint> <key>\n", argv[0]);
    }
    return fuse_main(argc, argv, &cryptfs_ops, NULL);
}

//run with: gcc -Wall mountfuse.c `pkg-config fuse --cflags --libs` -o mountfuse

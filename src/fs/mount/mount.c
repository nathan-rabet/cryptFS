#include "mount.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "format.h"
#include "print.h"

int mount_exists(const char *mount_point, const char * file_path)
{
    struct stat st;
    if (stat(mount_point, &st) == -1)
    {
        perror("stat");
        return -1; //The mount point does not exist or antoher error occured
    }

    if (is_already_formatted(file_path))
    {
        //Test if the mount point is a directory
        if (S_ISDIR(st.st_mode))
            print_success("The mount point `%s` exists.\n", mount_point);
        else
        {
            print_error("The mount point `%s` does not exist\n",
                        mount_point);
            return 1; //The mount point is not a directory
        }
    }
    else
    {
        print_error("The file `%s` is not formatted as a CryptFS.\n", file_path);
        return 1; //The file is not formatted as a CryptFS
    }
    return 0; //The file is formatted as a CryptFS and the mount point exist
}

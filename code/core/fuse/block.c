#include "block.h"

#include <stdio.h>

#include "cryptfs.h"

char *BLOCK_PATH = NULL;

int read_block(size_t block_num, char *buffer)
{
    FILE *file = fopen(BLOCK_PATH, "r");
    if (file == NULL)
        return -1;

    if (fseek(file, CRYPT_FS_ROOT_DIR_BLOCK + block_num * CRYPTFS_BLOCK_SIZE,
              SEEK_SET)
        != 0)
        return -1;

    size_t read = 0;
    while (read < CRYPTFS_BLOCK_SIZE)
    {
        size_t n = fread(buffer + read, 1, CRYPTFS_BLOCK_SIZE - read, file);
        if (n == 0)
            return -1;
        read += n;
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}

int write_block(size_t block_num, char *buffer)
{
    FILE *file = fopen(BLOCK_PATH, "r+");
    if (file == NULL)
        return -1;

    if (fseek(file, CRYPT_FS_ROOT_DIR_BLOCK + block_num * CRYPTFS_BLOCK_SIZE,
              SEEK_SET)
        == -1)
        return -1;

    size_t written = 0;
    while (written < CRYPTFS_BLOCK_SIZE)
    {
        size_t n =
            fwrite(buffer + written, 1, CRYPTFS_BLOCK_SIZE - written, file);
        if (n == 0)
            return -1;
        written += n;
    }

    if (fclose(file) == -1)
        return -1;

    return 0;
}

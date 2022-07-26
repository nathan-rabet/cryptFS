#include "block.h"

#include <stdio.h>

#include "cryptfs.h"

const char *BLOCK_PATH = NULL;

void set_device_path(const char *path)
{
    BLOCK_PATH = path;
}

const char *get_device_path()
{
    return BLOCK_PATH;
}

int read_blocks(size_t start_block, size_t nb_blocks, char *buffer)
{
    if (nb_blocks == 0)
        return 0;
    if (buffer == NULL)
        return -1;

    FILE *file = fopen(BLOCK_PATH, "r");
    if (file == NULL)
        return -1;

    if (fseek(file, start_block * CRYPTFS_BLOCK_SIZE_BYTES, SEEK_SET) != 0)
        return -1;

    size_t read = 0;
    while (read < CRYPTFS_BLOCK_SIZE_BYTES * nb_blocks)
    {
        size_t n = fread(buffer + read, 1,
                         (CRYPTFS_BLOCK_SIZE_BYTES * nb_blocks) - read, file);
        if (n == 0)
            return -1;
        read += n;
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}

int write_blocks(size_t start_block, size_t nb_blocks, char *buffer)
{
    if (nb_blocks == 0)
        return 0;
    if (buffer == NULL)
        return -1;

    FILE *file = fopen(BLOCK_PATH, "r+");
    if (file == NULL)
        return -1;

    if (fseek(file, start_block * CRYPTFS_BLOCK_SIZE_BYTES, SEEK_SET) == -1)
        return -1;

    size_t written = 0;
    while (written < CRYPTFS_BLOCK_SIZE_BYTES * nb_blocks)
    {
        size_t n =
            fwrite(buffer + written, 1,
                   (CRYPTFS_BLOCK_SIZE_BYTES * nb_blocks) - written, file);
        if (n == 0)
            return -1;
        written += n;
    }

    if (fclose(file) == -1)
        return -1;

    return 0;
}

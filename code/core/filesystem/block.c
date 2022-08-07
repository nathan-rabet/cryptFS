#include "block.h"

#include <assert.h>
#include <stdio.h>

#include "cryptfs.h"

const char *BLOCK_PATH = NULL;
uint32_t BLOCK_SIZE = 0;

void set_device_path(const char *path)
{
    BLOCK_PATH = path;
}

const char *get_device_path()
{
    assert(BLOCK_PATH != NULL);
    return BLOCK_PATH;
}

void set_block_size(const uint32_t size)
{
    assert(size >= CRYPTFS_BLOCK_SIZE_BYTES);
    BLOCK_SIZE = size;
}

uint32_t get_block_size()
{
    assert(BLOCK_SIZE != 0);
    return BLOCK_SIZE;
}

int read_blocks(size_t start_block, size_t nb_blocks, void *buffer)
{
    assert(BLOCK_PATH != NULL);
    assert(BLOCK_SIZE != 0);

    if (nb_blocks == 0)
        return 0;
    if (!buffer)
        return -1;

    FILE *file = fopen(BLOCK_PATH, "r");
    if (!file)
        return -1;

    if (fseek(file, start_block * BLOCK_SIZE, SEEK_SET) != 0)
        return -1;

    size_t read = 0;
    while (read < nb_blocks)
    {
        size_t n = fread(buffer + read * get_block_size(), get_block_size(),
                         nb_blocks - read, file);
        if (n == 0)
            return -1;
        read += n;
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}

int write_blocks(size_t start_block, size_t nb_blocks, void *buffer)
{
    assert(BLOCK_PATH != NULL);
    assert(BLOCK_SIZE != 0);

    if (nb_blocks == 0)
        return 0;
    if (buffer == NULL)
        return -1;

    FILE *file = fopen(BLOCK_PATH, "r+");
    if (file == NULL)
        return -1;

    if (fseek(file, start_block * get_block_size(), SEEK_SET) == -1)
        return -1;

    size_t written = 0;
    while (written < nb_blocks)
    {
        size_t n = fwrite(buffer + written * get_block_size(), get_block_size(),
                          nb_blocks - written, file);
        if (n == 0)
            return -1;
        written += n;
    }

    if (fclose(file) == -1)
        return -1;

    return 0;
}

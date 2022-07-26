#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>

extern char *DEVICE_PATH;

/**
 * @brief Set the device path global variable.
 *
 * @param path The path to the device.
 */
void set_device_path(const char *path);

/**
 * @brief Get the device path global variable.
 *
 * @return The device path global variable.
 */
const char *get_device_path();

/**
 * @brief Read blocks from the device.
 *
 * @param start_block The first block to read.
 * @param nb_blocks The number of blocks to read.
 * @param buffer The buffer to fill with the blocks.
 * (Must be allocated with at least CRYPTFS_BLOCK_SIZE_BYTES * nb_blocks bytes)
 *
 * @return 0 on success, -1 on error.
 */
int read_blocks(size_t start_block, size_t nb_blocks, void *buffer);

/**
 * @brief Write blocks to the device.
 *
 * @param start_block The first block to write.
 * @param nb_blocks The number of blocks to write.
 * @param buffer The buffer containing the blocks.
 *
 * @return 0 on success, -1 on error.
 */
int write_blocks(size_t start_block, size_t nb_blocks, void *buffer);

#endif /* BLOCK_H */

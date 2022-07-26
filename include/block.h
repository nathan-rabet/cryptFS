#ifndef BLOCK_H
#define BLOCK_H

#include <stddef.h>

extern char *BLOCK_PATH;

/**
 * @brief Reads a block content from a file.
 *
 * @param block_num The block to read.
 * @param buffer Buffer to store the block content.
 * Must be at least BLOCK_SIZE bytes long.
 * @return int 0 on success, -1 on error.
 */
int read_block(size_t block_num, char *buffer);

/**
 * @brief Writes the content of a buffer to a block.
 *
 * @param block_num The block to write to.
 * @param buffer The buffer from which to write.
 * @return int 0 on success, -1 on error.
 */
int write_block(size_t block_num, char *buffer);

#endif /* BLOCK_H */

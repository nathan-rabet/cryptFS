#ifndef FAT_H
#define FAT_H

#include "cryptfs.h"

/**
 * @brief Find the first block_type (in the enumeration) of the FAT linked-list.
 *
 * @param start_fat The first block of the FAT linked-list.
 * @param block_type The type of block to find.
 * @param corresponding_fat The block of the FAT linked-list where the
 * block_type is found.
 * @return int8_t
 */
int8_t find_first_fat_block(CryptFS_FAT *start_fat, uint8_t *block_type,
                            CryptFS_FAT **corresponding_fat);

#endif /* FAT_H */

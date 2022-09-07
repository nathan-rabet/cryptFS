#ifndef FAT_H
#define FAT_H

#include "cryptfs.h"

/**
 * @brief Find the first free block in the FAT table.
 *
 * @param first_fat The first block of the FAT linked-list.
 * @return int64_t The index of the first free block,
 * or FAT_BLOCK_ERROR first_fat is NULL or if an error occured.
 */
int64_t find_first_free_block(struct CryptFS_FAT *first_fat);

/**
 * @brief Append a FAT table to the FAT linked-list.
 *
 * @note The function still works if you pass any FAT table as first_fat
 * (not only the first one).
 *
 * @param first_fat A FAT table (can be any FAT table in the FAT linked-list).
 * @return int64_t The block where the new FAT is stored,
 * or FAT_BLOCK_ERROR if an error occurs.
 */
int64_t create_fat(struct CryptFS_FAT *any_fat);

/**
 * @brief Write `value` to the FAT table at `offset` index.
 *
 * @param first_fat The first block of the FAT table.
 * @param offset The index of the FAT table to write to.
 * @param value The value to write.
 * @return int 0 on success, FAT_BLOCK_ERROR on error.
 */
int write_fat_offset(struct CryptFS_FAT *first_fat, uint64_t offset,
                     uint64_t value);

/**
 * @brief Read the value at `offset` index in the FAT table.
 *
 * @param first_fat The first block of the FAT table.
 * @param offset The index of the FAT table to read from.
 * @return int64_t The value at `offset` index in the FAT table. FAT_BLOCK_ERROR
 * on error.
 */
int64_t read_fat_offset(struct CryptFS_FAT *first_fat, uint64_t offset);

#endif /* FAT_H */

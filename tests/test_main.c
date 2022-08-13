#include <assert.h>
#include <math.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "cryptfs.h"
#include "crypto.h"
#include "fat.h"
#include "format.h"
#include "print.h"
#include "xalloc.h"

int main(void)
{
    remove("find_first_free_block.on_second_fat.test.cfs");

    set_device_path(
        "find_first_free_block.on_second_fat_not_contigious.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    format_fs("find_first_free_block.on_second_fat_not_contigious.test.cfs");

    struct CryptFS_FAT *first_fat = xaligned_calloc(
        sizeof(struct CryptFS_FAT), 1, sizeof(struct CryptFS_FAT));

    struct CryptFS_FAT *second_fat = xaligned_calloc(
        sizeof(struct CryptFS_FAT), 1, sizeof(struct CryptFS_FAT));

    // Filling first FAT
    memset(first_fat->entries, '1',
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));
    first_fat->next_fat_table = ROOT_DIR_BLOCK + 2;

    // Filling second FAT
    memset(second_fat->entries, '2',
           (NB_FAT_ENTRIES_PER_BLOCK - 1) * sizeof(struct CryptFS_FAT_Entry));
    second_fat->next_fat_table = FAT_BLOCK_END;

    // Writing the FATs to the image
    write_blocks(FIRST_FAT_BLOCK, 1, first_fat);
    write_blocks(ROOT_DIR_BLOCK + 2, 1, second_fat);

    // Reading the structure from the file
    int64_t result = find_first_free_block(first_fat);
    print_info("result = %ld\n", result);

    return 0;
}

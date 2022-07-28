#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "block.h"
#include "cryptfs.h"
#include "fat.h"
#include "format.h"
#include "xalloc.h"

Test(find_first_free_block, not_found)
{
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));
    memset(cfs->first_fat.entries, 0xDEAD,
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));
    cfs->first_fat.next_fat_table = FAT_BLOCK_END;

    int64_t result = find_first_free_block(&cfs->first_fat);
    cr_assert_eq(result, -1, "result = %ld", result);

    free(cfs);
}

Test(find_first_free_block, on_first_fat)
{
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));
    memset(cfs->first_fat.entries, 0xDEAD,
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));

    size_t index = 42;
    cr_assert(index < NB_FAT_ENTRIES_PER_BLOCK);

    cfs->first_fat.entries[index].next_block = FAT_BLOCK_FREE;

    int64_t result = find_first_free_block(&cfs->first_fat);
    cr_assert_eq(result, index, "result = %ld", result);

    free(cfs);
}

Test(find_first_free_block, on_second_fat)
{
    struct CryptFS *cfs =
        xcalloc(1, sizeof(struct CryptFS) + sizeof(struct CryptFS_FAT));

    struct CryptFS_FAT *second_fat =
        (struct CryptFS_FAT *)((char *)cfs + sizeof(struct CryptFS));

    // Filling first FAT
    memset(cfs->first_fat.entries, 0xDEADBEEF,
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));
    cfs->first_fat.next_fat_table = ROOT_DIR_BLOCK + 1;

    // Filling second FAT
    second_fat->next_fat_table = FAT_BLOCK_END;

    // Writing the structure to a file
    FILE *fp = fopen("build/find_first_free_block.on_second_fat.test.cfs", "w");
    if (fwrite(cfs, sizeof(struct CryptFS) + sizeof(struct CryptFS_FAT), 1, fp)
        != 1)
        cr_assert(false, "Failed to write the structure to a file");
    fclose(fp);

    // Setting the device and block size for read/write operations
    set_device_path("build/find_first_free_block.on_second_fat.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    // Reading the structure from the file
    int64_t result = find_first_free_block(&cfs->first_fat);
    cr_assert_eq(result, NB_FAT_ENTRIES_PER_BLOCK, "result = %ld", result);

    free(cfs);
}

Test(find_first_free_block, on_second_fat_not_contigious)
{
    struct CryptFS *cfs =
        xcalloc(1, sizeof(struct CryptFS) + 2 * sizeof(struct CryptFS_FAT));

    struct CryptFS_FAT *second_fat =
        (struct CryptFS_FAT *)((char *)cfs + sizeof(struct CryptFS)
                               + sizeof(struct CryptFS_FAT));

    // Filling first FAT
    memset(cfs->first_fat.entries, 0xDEADBEEF,
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));
    cfs->first_fat.next_fat_table = ROOT_DIR_BLOCK + 2;

    // Filling second FAT
    second_fat->next_fat_table = FAT_BLOCK_END;

    // Writing the structure to a file
    FILE *fp = fopen("build/find_first_free_block.on_second_fat.test.cfs", "w");
    if (fwrite(cfs, sizeof(struct CryptFS) + 2 * sizeof(struct CryptFS_FAT), 1,
               fp)
        != 1)
        cr_assert(false, "Failed to write the structure to a file");
    fclose(fp);

    // Setting the device and block size for read/write operations
    set_device_path("build/find_first_free_block.on_second_fat.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    // Reading the structure from the file
    int64_t result = find_first_free_block(&cfs->first_fat);
    cr_assert_eq(result, NB_FAT_ENTRIES_PER_BLOCK, "result = %ld", result);

    // Deleting the file
    if (remove("build/find_first_free_block.on_second_fat.test.cfs") != 0)
        cr_assert(false, "Failed to remove the file");

    free(cfs);
}

Test(create_fat, second_fat, .init = cr_redirect_stdout, .timeout = 10)
{
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));

    // Setting the device and block size for read/write operations
    set_device_path("build/create_fat.second_fat.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    format_fs("build/create_fat.second_fat.test.cfs");

    FILE *fp = fopen("build/create_fat.second_fat.test.cfs", "r");
    if (fread(cfs, sizeof(struct CryptFS), 1, fp) != 1)
        cr_assert(false, "Failed to read the structure from a file");
    fclose(fp);

    // Reading the structure from the file
    int64_t result = create_fat(&cfs->first_fat);
    cr_assert_eq(result, ROOT_DIR_BLOCK + 1, "result = %ld", result);

    // Deleting the file
    if (remove("build/create_fat.second_fat.test.cfs") != 0)
        cr_assert(false, "Failed to remove the file");

    free(cfs);
}

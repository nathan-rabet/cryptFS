#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "cryptfs.h"
#include "fat.h"
#include "xalloc.h"

Test(find_first_free_block, on_first_fat)
{
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));
    memset(cfs->first_fat.entries, 0xDEAD,
           NB_FAT_ENTRIES_PER_BLOCK * sizeof(struct CryptFS_FAT_Entry));

    size_t index = 42;
    cfs->first_fat.entries[index].next_block = FAT_BLOCK_FREE;

    int64_t result = find_first_free_block(&cfs->first_fat);
    cr_assert_eq(result, index, "result = %ld", result);
    cr_assert(index < NB_FAT_ENTRIES_PER_BLOCK);

    free(cfs);
}

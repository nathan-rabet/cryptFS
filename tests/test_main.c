#include <assert.h>
#include <math.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "cryptfs.h"
#include "crypto.h"
#include "errors.h"
#include "fat.h"
#include "format.h"
#include "xalloc.h"

int main(void)
{
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));
    memset(&cfs->first_fat.entries->next_block, 0xDEAD,
           NB_FAT_ENTRIES_PER_BLOCK);

    size_t index = 42;
    cfs->first_fat.entries[index].next_block = FAT_BLOCK_FREE;

    int64_t result = find_first_free_block(&cfs->first_fat);

    (void)result;

    return 0;
}

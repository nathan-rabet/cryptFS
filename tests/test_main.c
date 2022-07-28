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

    // Setting the device and block size for read/write operations
    set_device_path("build/create_fat.second_fat.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    format_fs("build/create_fat.second_fat.test.cfs");

    FILE *fp = fopen("build/create_fat.second_fat.test.cfs", "r");
    if (fread(cfs, sizeof(struct CryptFS), 1, fp) != 1)
        return 1;
    fclose(fp);

    // Reading the structure from the file
    int64_t result = create_fat(&cfs->first_fat);

    printf("result = %ld\n", result);

    // Deleting the file
    if (remove("build/create_fat.second_fat.test.cfs") != 0)
        return 1;

    free(cfs);

    return 0;
}

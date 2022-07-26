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
#include "format.h"
#include "xalloc.h"

int main(void)
{
    struct CryptFS *cfs_before = xcalloc(1, sizeof(struct CryptFS));
    struct CryptFS *cfs_after = xcalloc(1, sizeof(struct CryptFS));

    format_fill_filesystem_struct(cfs_before);

    // Write the CryptFS to the file
    FILE *file = fopen("integrity.test.cfs", "w");
    if (file == NULL || fwrite(cfs_before, sizeof(*cfs_before), 1, file) != 1)
        error_exit("Impossible to write the filesystem structure on the disk",
                   EXIT_FAILURE);
    fclose(file);

    // Set the device (global variable) to the file (used by read/write_blocks)
    set_device_path("integrity.test.cfs");

    // Read the the CryptFS
    read_blocks(0, 67, cfs_after);
    // Compare the two cfs
    for (size_t i = 0; i < sizeof(struct CryptFS); i++)
        if (((char *)cfs_before)[i] != ((char *)cfs_after)[i])
        {
            // Print the first 10 byte that are different
            for (size_t j = 0; j < 10; j++)
                printf("%02x != %02x\n", ((char *)cfs_before)[i + j],
                       ((char *)cfs_after)[i + j]);
            assert(0);
        }

    return 0;
}

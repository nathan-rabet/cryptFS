#include <assert.h>
#include <errno.h>
#include <math.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "block.h"
#include "cryptfs.h"
#include "crypto.h"
#include "fat.h"
#include "format.h"
#include "print.h"
#include "xalloc.h"

int main(void)
{
    set_device_path("block_read_write_with_encryption.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    format_fs("block_read_write_with_encryption.test.cfs", NULL);

    unsigned char *aes_key = generate_aes_key();

    unsigned char *buffer_before_encryption = xcalloc(1, get_block_size() + 1);
    unsigned char *buffer_after_decryption = xcalloc(1, get_block_size() + 1);

    assert(RAND_bytes(buffer_before_encryption, get_block_size()));

    int ret =
        write_blocks_with_encryption(aes_key, 0, 1, buffer_before_encryption);
    assert(ret == 0);
    ret = read_blocks_with_decryption(aes_key, 0, 1, buffer_after_decryption);
    assert(ret == 0);
    // cr_assert_arr_eq(buffer_before_encryption, buffer_after_decryption,
    //                  get_block_size());
    free(aes_key);
    free(buffer_before_encryption);
    free(buffer_after_decryption);

    return 0;
}

#include <assert.h>
#include <math.h>
#include <openssl/evp.h>
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
    struct CryptFS_Key *keys_storage = xcalloc(1, sizeof(struct CryptFS_Key));

    EVP_PKEY *rsa_keypair = generate_rsa_keypair();
    EVP_PKEY *rsa_keypair_different = generate_rsa_keypair();

    unsigned char *aes_key = generate_aes_key();

    store_keys_in_keys_storage(keys_storage, rsa_keypair, aes_key);

    if (find_rsa_matching_key(rsa_keypair_different, keys_storage) == -1)
        printf("find_rsa_matching_key: OK\n");
    else
        printf("find_rsa_matching_key: FAIL\n");

    free(aes_key);
    EVP_PKEY_free(rsa_keypair);
    EVP_PKEY_free(rsa_keypair_different);
    free(keys_storage);

    return 0;
}

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <openssl/evp.h>
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
    remove("crypto_disk__load_rsa_keypair_from_disk");
    int dir_res = mkdir("crypto_disk__load_rsa_keypair_from_disk", 0755);
    if (dir_res != 0 && errno != EEXIST)
        assert(false);

    EVP_PKEY *rsa_keypair = generate_rsa_keypair();
    write_rsa_keys_on_disk(rsa_keypair,
                           "crypto_disk__load_rsa_keypair_from_disk", NULL);
    EVP_PKEY *rsa_keypair_loaded = load_rsa_keypair_from_disk(
        "crypto_disk__load_rsa_keypair_from_disk/.cryptfs/public.pem",
        "crypto_disk__load_rsa_keypair_from_disk/.cryptfs/private.pem", NULL);
    if (EVP_PKEY_eq(rsa_keypair, rsa_keypair_loaded) == 1)
        printf("Keys are equal\n");
    else
        printf("Keys are different\n");
    EVP_PKEY_free(rsa_keypair);
    EVP_PKEY_free(rsa_keypair_loaded);

    return 0;
}

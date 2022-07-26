#include <assert.h>
#include <math.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cryptfs.h"
#include "crypto.h"
#include "format.h"
#include "xalloc.h"

int main(void)
{
    struct CryptFS_Header *header = xcalloc(1, sizeof(struct CryptFS_Header));

    RSA *rsa_keypair = RSA_new();
    unsigned char *aes_key = xcalloc(1, RSA_KEY_SIZE_BYTES + 1);

    generate_keys(aes_key, rsa_keypair);
    store_keys_in_headers(header, rsa_keypair, aes_key);

    int8_t index = find_rsa_matching_key(rsa_keypair, header);

    printf("The index of the matching RSA keypair is %d\n", index);

    free(aes_key);
    RSA_free(rsa_keypair);
    free(header);

    return 0;
}

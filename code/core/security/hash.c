#include "hash.h"

#include <openssl/evp.h>
#include <openssl/rsa.h>

#include "cryptfs.h"
#include "xalloc.h"

unsigned char *sha256_data(void *data, size_t len_data)
{
    unsigned char *hash = xmalloc(EVP_MAX_MD_SIZE, 1);

    EVP_MD_CTX *sha256_ctx = EVP_MD_CTX_create();
    EVP_DigestInit(sha256_ctx, EVP_sha256());
    EVP_DigestUpdate(sha256_ctx, data, len_data);
    EVP_DigestFinal(sha256_ctx, hash, NULL);
    EVP_MD_CTX_destroy(sha256_ctx);

    return hash;
}

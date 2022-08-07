#include "hash.h"

#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "cryptfs.h"
#include "xalloc.h"

unsigned char *sha256_data(void *data, size_t len_data)
{
    unsigned char *hash = xmalloc(SHA384_DIGEST_LENGTH, 1);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, len_data);
    SHA256_Final(hash, &sha256);

    return hash;
}

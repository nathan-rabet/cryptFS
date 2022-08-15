#include <openssl/evp.h>
#include <openssl/pem.h>

#include "crypto.h"
#include "print.h"

EVP_PKEY *load_rsa_keypair_from_disk(const char *public_key_path,
                                     const char *private_key_path,
                                     char *passphrase)
{
    EVP_PKEY *rsa_keypair = NULL;

    FILE *public_key_file = fopen(public_key_path, "r");
    FILE *private_key_file = fopen(private_key_path, "r");

    if (!public_key_file || !private_key_file)
        return NULL;

    if (PEM_read_PUBKEY(public_key_file, &rsa_keypair, NULL, NULL) == NULL)
        internal_error_exit("Failed to load public key\n", EXIT_FAILURE);
    if (PEM_read_PrivateKey(private_key_file, &rsa_keypair, NULL, passphrase)
        == NULL)
        internal_error_exit("Failed to load private key\n", EXIT_FAILURE);
    return rsa_keypair;
}

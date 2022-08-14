#include "crypto.h"

#include <openssl/err.h>

#include "print.h"
#include "xalloc.h"

unsigned char *rsa_encrypt_data(EVP_PKEY *rsa_key, const unsigned char *data,
                                size_t data_size, size_t *encrypted_data_size)
{
    // Setting up EVP CTX
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new(rsa_key, NULL);
    if (pctx == NULL || EVP_PKEY_encrypt_init(pctx) <= 0)
        internal_error_exit("Failed to initialize RSA encryption\n",
                            EXIT_FAILURE);

    // Determining the size of the output buffer
    if (EVP_PKEY_encrypt(pctx, NULL, encrypted_data_size, data, data_size) != 1)
        internal_error_exit(
            "Failed to determine the size of the output buffer\n",
            EXIT_FAILURE);

    unsigned char *encrypted_data = xcalloc(1, *encrypted_data_size);
    if (EVP_PKEY_encrypt(pctx, encrypted_data, encrypted_data_size, data,
                         data_size)
        != 1)
        internal_error_exit("Failed to encrypt data\n", EXIT_FAILURE);
    EVP_PKEY_CTX_free(pctx);
    return encrypted_data;
}

unsigned char *rsa_decrypt_data(EVP_PKEY *rsa_key,
                                const unsigned char *encrypted_data,
                                size_t encrypted_data_size,
                                size_t *decrypted_data_size)
{
    // Setting up EVP CTX
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new(rsa_key, NULL);
    if (pctx == NULL || EVP_PKEY_decrypt_init(pctx) <= 0)
        internal_error_exit("Failed to initialize RSA decryption\n",
                            EXIT_FAILURE);

    // Determining the size of the output buffer
    if (EVP_PKEY_decrypt(pctx, NULL, decrypted_data_size, encrypted_data,
                         encrypted_data_size)
        != 1)
        internal_error_exit(
            "Failed to determine the size of the output buffer\n",
            EXIT_FAILURE);

    unsigned char *decrypted_data = xcalloc(1, *decrypted_data_size);
    if (EVP_PKEY_decrypt(pctx, decrypted_data, decrypted_data_size,
                         encrypted_data, encrypted_data_size)
        != 1)
        internal_error_exit("Failed to decrypt data\n", EXIT_FAILURE);
    EVP_PKEY_CTX_free(pctx);
    return decrypted_data;
}

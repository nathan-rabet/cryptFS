#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/rsa.h>

#include "cryptfs.h"

#define RSA_EXPONENT 3

/**
 * @brief Generates a new RSA keypair + an AES key.
 *
 * @param aes_key The AES key which is generated.
 * @param rsa_keypair The RSA keypair which is generated.
 */
void generate_keys(unsigned char *aes_key, RSA *rsa_keypair);

/**
 * @brief Stores the RSA modulus and the RSA public exponent in the header.
 *
 * @param header
 * @param rsa_keypair
 * @param aes_key
 */
void store_keys_in_headers(struct CryptFS_Header *header, RSA *rsa_keypair,
                           unsigned char *aes_key);

/**
 * @brief Find the key in the header which matches the given RSA keypair (user
 * one).
 *
 * @param rsa_private The RSA of the user
 * @param header The header to search in
 * @return int8_t The index of the key in the header, -1 if not found.
 */
int8_t find_rsa_matching_key(RSA *rsa_private, struct CryptFS_Header *header);

#endif /* CRYPTO_H */

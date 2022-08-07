#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/rsa.h>

#include "cryptfs.h"

#define RSA_EXPONENT 65537

/**
 * @brief Generates a new RSA keypair + an AES key.
 *
 * @param aes_key The AES key which is generated (returned).
 * @param rsa_keypair The RSA keypair which is generated (returned).
 */
void generate_keys(unsigned char *aes_key, RSA *rsa_keypair);

/**
 * @brief Stores the RSA modulus and the RSA public exponent in a keys storage.
 *
 * @param keys_storage The keys storage.
 * @param rsa_keypair The RSA keypair: modulus and public exponent will be
 * stored.
 * @param aes_key The AES key: RSAPUB_Encrypt(aes_key) will be stored.
 */
void store_keys_in_keys_storage(struct CryptFS_Key *keys_storage,
                                RSA *rsa_keypair, unsigned char *aes_key);

/**
 * @brief Writes the RSA private and public keys to a file.
 *
 * @param rsa_keypair The RSA keypair which is written.
 * @param path_to_write The path to the file which is written.
 */
void write_rsa_keys_on_disk(RSA *rsa_keypair, const char *path_to_write);

/**
 * @brief Find the key in the keys storage which matches the given RSA keypair
 * (user one).
 *
 * @param rsa_private The RSA of the user.
 * @param keys_storage The keys storage to search in.
 * @return int8_t The index of the key in the header, -1 if not found.
 */
int8_t find_rsa_matching_key(RSA *rsa_private,
                             struct CryptFS_Key *keys_storage);

#endif /* CRYPTO_H */

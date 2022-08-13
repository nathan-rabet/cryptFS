#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/rsa.h>

#include "cryptfs.h"

#define RSA_EXPONENT RSA_F4

/**
 * @brief Generates a random AES key.
 *
 * @return unsigned char* The generated AES key.
 */
unsigned char *generate_aes_key(void);

/**
 * @brief Generates a random RSA keypair.
 *
 * @return EVP_PKEY* The generated RSA keypair.
 */
EVP_PKEY *generate_rsa_keypair(void);

/**
 * @brief Stores the RSA modulus and the RSA public exponent in a keys storage.
 *
 * @param keys_storage The keys storage.
 * @param rsa_keypair The RSA keypair: modulus and public exponent will be
 * stored.
 * @param aes_key The AES key: RSAPUB_Encrypt(aes_key) will be stored.
 */
void store_keys_in_keys_storage(struct CryptFS_Key *keys_storage,
                                EVP_PKEY *rsa_keypair, unsigned char *aes_key);

/**
 * @brief Writes the RSA private and public keys to a file.
 *
 * @param rsa_keypair The RSA keypair which is written.
 * @param path_to_write The path to the file which is written.
 * @param passphrase The passphrase used to encrypt the keys.
 */
void write_rsa_keys_on_disk(EVP_PKEY *rsa_keypair, const char *path_to_write,
                            char *passphrase);

/**
 * @brief Find the key in the keys storage which matches the given RSA
 * keypair (user one).
 *
 * @param rsa_private The RSA of the user.
 * @param keys_storage The keys storage to search in.
 * @return int8_t The index of the key in the header, -1 if not found.
 */
int8_t find_rsa_matching_key(EVP_PKEY *rsa_private,
                             struct CryptFS_Key *keys_storage);

#endif /* CRYPTO_H */

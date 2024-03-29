#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/evp.h>

#include "cryptfs.h"

#define RSA_EXPONENT RSA_F4

/**
 * @brief Encrypts `data` of size `data_size` with `rsa_key` public key.
 *
 * @param rsa_key The RSA (public) key to use for encryption.
 * @param data The data to encrypt.
 * @param data_size The size of the data to encrypt.
 * @param encrypted_data_size The size of the encrypted data (returned).
 * @return unsigned char* The encrypted data.
 */
unsigned char *rsa_encrypt_data(EVP_PKEY *rsa_key, const unsigned char *data,
                                size_t data_size, size_t *encrypted_data_size);

/**
 * @brief Decrypts `encrypted_data` of size `encrypted_data_size` with
 * `rsa_key` private key.
 *
 * @param rsa_key The RSA (private) key to use for decryption.
 * @param encrypted_data The data to decrypt.
 * @param encrypted_data_size The size of the data to decrypt.
 * @param decrypted_data_size The size of the decrypted data (returned).
 * @return unsigned char* The decrypted data.
 */
unsigned char *rsa_decrypt_data(EVP_PKEY *rsa_key,
                                const unsigned char *encrypted_data,
                                size_t encrypted_data_size,
                                size_t *decrypted_data_size);

/**
 * @brief Encrypts `data` of size `data_size` with `aes_key` key.
 *
 * @param aes_key The AES key to use for encryption.
 * @param data The data to encrypt.
 * @param data_size The size of the data to encrypt.
 * @param encrypted_data_size The size of the encrypted data (returned).
 * @return unsigned char* The encrypted data.
 */
unsigned char *aes_encrypt_data(unsigned char *aes_key,
                                const unsigned char *data, size_t data_size,
                                size_t *encrypted_data_size);

/**
 * @brief Decrypts `encrypted_data` of size `encrypted_data_size` with
 * `aes_key` key.
 *
 * @param aes_key The AES key to use for decryption.
 * @param encrypted_data The data to decrypt.
 * @param encrypted_data_size The size of the data to decrypt.
 * @param decrypted_data_size The size of the decrypted data (returned).
 * @return unsigned char* The decrypted data.
 */
unsigned char *aes_decrypt_data(unsigned char *aes_key,
                                const unsigned char *encrypted_data,
                                size_t encrypted_data_size,
                                size_t *decrypted_data_size);

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

/**
 * @brief Loads the RSA private and public keys from the given file.
 *
 * @param public_key_path The path to the public key file.
 * @param private_key_path The path to the private key file.
 * @param passphrase The passphrase used to encrypt the keys,
 * NULL if no passphrase is used.
 * @return EVP_PKEY* The loaded RSA keypair.
 */
EVP_PKEY *load_rsa_keypair_from_disk(const char *public_key_path,
                                     const char *private_key_path,
                                     char *passphrase);

/**
 * @brief Loads the RSA private and public keys from the current user's home
 * directory.
 *
 * @param passphrase Gets filled with the passphrase the user enters if he
 * decides to.
 * @return EVP_PKEY* The loaded RSA keypair.
 */
EVP_PKEY *load_rsa_keypair_from_home(char **passphrase);

#endif /* CRYPTO_H */

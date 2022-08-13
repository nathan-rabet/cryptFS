#ifndef PASSPHRASE_H
#define PASSPHRASE_H

#include <stdbool.h>

/**
 * @brief Ask user for passphrase (only print '*' for each char on stdout)
 */
char *ask_user_passphrase(void);

/**
 * @brief Check if a PEM encoded RSA private key has a passphrase
 *
 * @param rsa_path Path to the PEM RSA private key
 * @return true if the key has a passphrase, false otherwise
 */
bool rsa_private_is_encrypted(const char *rsa_path);

#endif /* PASSPHRASE_H */

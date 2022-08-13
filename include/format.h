#ifndef FORMAT_H
#define FORMAT_H

#include <stdbool.h>

#include "cryptfs.h"

/**
 * @brief Check if the given file is a cryptfs file system.
 *
 * @param file_path Path to the file to check.
 * @return true if the file is a cryptfs file system.
 * @return false if the file is not a cryptfs file system.
 */
bool is_already_formatted(const char *file_path);

/**
 * @brief File the `struct CryptFS` structure when first formatting the
 * filesystem.
 *
 * @param cfs The `struct CryptFS` structure to fill.
 * @param rsa_passphrase The passphrase used to encrypt the RSA private key.
 * Set to NULL if no passphrase is needed.
 */
void format_fill_filesystem_struct(struct CryptFS *cfs, char *rsa_passphrase);

/**
 * @brief Format the given file to a cryptfs file system.
 *
 * @param path The path to the file to format.
 * @param rsa_passphrase The passphrase used to encrypt the RSA private key.
 * Set to NULL if no passphrase is needed.
 */
void format_fs(const char *path, char *rsa_passphrase);

#endif /* FORMAT_H */

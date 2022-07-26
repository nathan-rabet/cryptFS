#include "format.h"

#include <openssl/rsa.h>

#include "block.h"
#include "cryptfs.h"
#include "crypto.h"
#include "errors.h"
#include "xalloc.h"

bool is_already_formatted(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return false;

    struct CryptFS_Header header = { 0 };

    fread(&header, sizeof(header), 1, file);

    // Check if the magic number is correct
    if (header.magic != CRYPTFS_MAGIC)
        return false;
    // Check if the version is correct
    else if (header.version != CRYPTFS_VERSION)
        return false;
    // Check if the blocksize is correct (must be a multiple of 2)
    else if (header.blocksize == 0 || header.blocksize % 2 != 0)
        return false;

    return true;
}

void format_fs(const char *path)
{
    // Check if the file is already formatted
    if (is_already_formatted(path))
    {
        // Ask the user if he wants to overwrite the file
        printf("The file is already formatted. Do you want to overwrite it? "
               "(y/n) ");
        char answer = getchar();
        if (answer != 'y' && answer != 'Y')
        {
            printf("Aborting...\n");
            return;
        }
        else
            printf("Overwriting...\n");
    }

    FILE *file = fopen(path, "w+");
    if (file == NULL)
    {
        perror("Impossible to open the file for formatting");
        exit(EXIT_FAILURE);
    }

    // Craft the header
    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));
    cfs->header.magic = CRYPTFS_MAGIC;
    cfs->header.version = CRYPTFS_VERSION;
    cfs->header.blocksize = CRYPTFS_BLOCK_SIZE_BYTES;
    cfs->header.boot[0] = 0x90;
    cfs->header.boot[1] = 0x90;
    cfs->header.boot[2] = 0x90;
    cfs->header.boot[3] = 0x90;
    cfs->header.boot[4] = 0x90;

    // Generate AES + RSA keys
    unsigned char aes_key[RSA_KEY_SIZE_BYTES] = { 0 };
    RSA *keypair = RSA_new();
    generate_keys(aes_key, keypair);

    // Store the RSA modulus and the RSA public exponent in the header
    store_keys_in_keys_storage(cfs->keys_storage, keypair, aes_key);

    // Get user directory path
    char *user_dir_path = getenv("HOME");
    if (!user_dir_path)
        error_exit("Impossible to get the user directory path", EXIT_FAILURE);

    write_rsa_keys_on_disk(keypair, user_dir_path);

    // Set the device path
    set_device_path(path);

    // Write the header (at block 0)
    if (write_blocks(0, 1, (char *)&cfs->header) == -1)
        error_exit("Impossible to write the header", EXIT_FAILURE);

    // Write CRYPTS_BLOCK_SIZE bytes of 0x00 on block 1 and 2
    // (the first FAT and the root directory)
    char *zero = xcalloc(1, CRYPTFS_BLOCK_SIZE_BYTES);
    if (write_blocks(1, 1, zero) != 0 || write_blocks(2, 1, zero) != 0)
        error_exit("Impossible to write zero's on block 1 or block 2",
                   EXIT_FAILURE);

    free(cfs);
    free(zero);
    RSA_free(keypair);

    fclose(file);
}

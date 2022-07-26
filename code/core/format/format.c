#include "format.h"

#include <openssl/rsa.h>

#include "cryptfs.h"
#include "crypto.h"
#include "errors.h"

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
    struct CryptFS_Header header = {
        .boot = { 0x90, 0x90, 0x90, 0x90, 0x90 }, // Nop sled
        .magic = CRYPTFS_MAGIC,
        .version = CRYPTFS_VERSION,
        .blocksize = CRYPTFS_BLOCK_SIZE,
        .fat = { .next_fat_block = FAT_BLOCK_END, .entries = { 0 } },
    };
    header.fat.entries[0] =
        FAT_BLOCK_RESERVED; // Root directory is the first entry of the FAT

    // Generate AES + RSA keys
    unsigned char aes_key[RSA_KEY_SIZE_BYTES] = { 0 };
    RSA *keypair = RSA_new();
    generate_keys(aes_key, keypair);

    // Store the RSA modulus and the RSA public exponent in the header
    store_keys_in_headers(&header, keypair, aes_key);

    // Get user directory path
    char *user_dir_path = getenv("HOME");
    if (!user_dir_path)
    {
        perror("Impossible to get the user directory path");
        exit(EXIT_FAILURE);
    }
    write_rsa_keys_on_disk(keypair, user_dir_path);

    // Write the header
    if (fwrite(&header, sizeof(struct CryptFS_Header), 1, file) != 1)
    {
        perror("Impossible to write the header");
        exit(EXIT_FAILURE);
    }
    RSA_free(keypair);
    fclose(file);
}

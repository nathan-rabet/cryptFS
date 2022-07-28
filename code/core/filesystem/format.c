#include "format.h"

#include <openssl/rsa.h>

#include "block.h"
#include "cryptfs.h"
#include "crypto.h"
#include "errors.h"
#include "fat.h"
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

void format_fill_filesystem_struct(struct CryptFS *cfs)
{
    /// ------------------------------------------------------------
    /// BLOCK 0 : HEADER
    /// ------------------------------------------------------------

    // Craft the header
    cfs->header.magic = CRYPTFS_MAGIC;
    cfs->header.version = CRYPTFS_VERSION;
    cfs->header.blocksize = CRYPTFS_BLOCK_SIZE_BYTES;

    for (size_t i = 0; i < CRYPTFS_BOOT_SECTION_SIZE_BYTES; i++)
        cfs->header.boot[i] = 0x90; // NOP sled

    /// ------------------------------------------------------------
    /// BLOCK 1 : KEYS STORAGE
    /// ------------------------------------------------------------

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

    // Write the RSA public and private keys in the user directory
    write_rsa_keys_on_disk(keypair, user_dir_path);

    /// ------------------------------------------------------------
    /// BLOCK 2 : FAT (File Allocation Table)
    /// ------------------------------------------------------------

    cfs->first_fat.next_fat_table = FAT_BLOCK_END;
    for (size_t i = 0; i <= ROOT_DIR_BLOCK; i++)
        write_fat_offset(&cfs->first_fat, i, FAT_BLOCK_END);

    /// ------------------------------------------------------------
    /// BLOCK 3 : ROOT DIRECTORY
    /// ------------------------------------------------------------
    //// Noting to add

    RSA_free(keypair);
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

    struct CryptFS *cfs = xcalloc(1, sizeof(struct CryptFS));

    format_fill_filesystem_struct(cfs);

    FILE *file = fopen(path, "w+");
    if (file == NULL)
        error_exit("Impossible to open the file", EXIT_FAILURE);

    // Write the filesystem structure on the disk
    printf("Writing the filesystem structure on the disk...\n");
    fwrite(cfs, sizeof(*cfs), 1, file);

    free(cfs);

    fclose(file);
}

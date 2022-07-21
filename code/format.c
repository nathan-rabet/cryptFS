#include "format.h"

#include <assert.h>
#include <errno.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cryptfs.h"

#define STR(x) #x
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define RSA_EXPONENT 3

bool is_already_formatted(const char *path)
{
    FILE *file = fopen(path, "r");
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
        FAT_BLOCK_END; // Root directory is the first entry of the FAT

    // Generate the AES key
    printf("Generating the AES key (for the filesystem encryption)...\n");
    unsigned char aes_key[RSA_KEY_SIZE_BYTES];
    if (RAND_bytes(aes_key, RSA_KEY_SIZE_BYTES) != 1)
    {
        perror("Impossible to generate the AES key");
        exit(EXIT_FAILURE);
    }

    // Generating a RSA keypair for the encryption
    printf("Generating a RSA keypair (for the AES key encryption)...\n");
    RSA *keypair = RSA_new();
    BIGNUM *exponent = BN_new();
    if (!keypair || !exponent || !BN_set_word(exponent, RSA_EXPONENT))
    {
        perror("Impossible to generate the RSA keypair");
        exit(EXIT_FAILURE);
    }
    if (RSA_generate_key_ex(keypair, RSA_KEY_SIZE_BITS, exponent, NULL) != 1)
    {
        perror("Impossible to generate the RSA keypair");
        exit(EXIT_FAILURE);
    }

    // Store the RSA modulus and the RSA public exponent in the header
    const BIGNUM *modulus = RSA_get0_n(keypair);

    if (!modulus || !BN_bn2bin(RSA_get0_n(keypair), header.keys[0].rsa_n))
    {
        perror("Impossible to store the RSA modulus");
        exit(EXIT_FAILURE);
    }
    header.keys[0].rsa_e = RSA_EXPONENT;

    // Store the encrypted AES key in the header
    printf("Encrypting the AES key with the RSA keypair...\n");
    int encrypted_aes_key_size = RSA_public_encrypt(
        RSA_KEY_SIZE_BYTES, aes_key, header.keys[0].aes_key_ciphered, keypair,
        RSA_NO_PADDING);
    if (encrypted_aes_key_size == -1)
    {
        perror("Impossible to encrypt the AES key");
        exit(EXIT_FAILURE);
    }

    assert(encrypted_aes_key_size == RSA_KEY_SIZE_BYTES);

    // Get user directory path
    char *user_dir_path = getenv("HOME");
    if (user_dir_path == NULL)
    {
        perror("Impossible to get the user directory path");
        exit(EXIT_FAILURE);
    }

    // Create the paths:
    // - user_dir_path + "/.cryptfs"
    // - user_dir_path + "/.cryptfs/public.pem"
    // - user_dir_path + "/.cryptfs/private.pem"
    char *cryptfs_path =
        malloc(strlen(user_dir_path) + strlen("/.cryptfs") + 1);
    strcpy(cryptfs_path, user_dir_path);
    strcat(cryptfs_path, "/.cryptfs");

    char *public_pem_path =
        malloc(strlen(user_dir_path) + strlen("/.cryptfs/public.pem") + 1);
    strcpy(public_pem_path, user_dir_path);
    strcat(public_pem_path, "/.cryptfs/public.pem");

    char *private_pem_path =
        malloc(strlen(user_dir_path) + strlen("/.cryptfs/private.pem") + 1);
    strcpy(private_pem_path, user_dir_path);
    strcat(private_pem_path, "/.cryptfs/private.pem");

    // Create the directories
    printf("Creating the directories...\n");
    if (mkdir(cryptfs_path, 0755) != 0 && errno != EEXIST)
    {
        perror("Impossible to create the directories");
        exit(EXIT_FAILURE);
    }

    // Store the RSA private in ~/.cryptfs/private.pem
    printf("Storing the RSA private key in ~/.cryptfs/private.pem...\n");
    FILE *private_key = fopen(private_pem_path, "w+");
    if (private_key == NULL)
    {
        perror("Impossible to open the private key file");
        exit(EXIT_FAILURE);
    }
    if (PEM_write_RSAPrivateKey(private_key, keypair, NULL, NULL, 0, NULL, NULL)
            != 1
        || fclose(private_key) != 0)
        exit(EXIT_FAILURE);

    // Store the RSA public in ~/.cryptfs/public.pem
    printf("Storing the RSA public key in ~/.cryptfs/public.pem...\n");
    FILE *public_key = fopen(public_pem_path, "w+");
    if (public_key == NULL)
    {
        perror("Impossible to open the public key file");
        exit(EXIT_FAILURE);
    }

    if (PEM_write_RSA_PUBKEY(public_key, keypair) != 1
        || fclose(public_key) != 0)
        exit(EXIT_FAILURE);

    // Write the header
    if (fwrite(&header, sizeof(struct CryptFS_Header), 1, file) != 1)
    {
        perror("Impossible to write the header");
        exit(EXIT_FAILURE);
    }
    free(cryptfs_path);
    free(public_pem_path);
    free(private_pem_path);
    RSA_free(keypair);
    BN_free(exponent);
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    format_fs(argv[1]);
    printf("Done!\n");
    return 0;
}

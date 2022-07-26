#include <assert.h>
#include <errno.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cryptfs.h"
#include "crypto.h"
#include "errors.h"
#include "xalloc.h"

#define STR(x) #x
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void generate_keys(unsigned char *aes_key, RSA *rsa_keypair)
{
    // Generate AES key
    printf("Generating the AES key (for the filesystem encryption)...\n");
    if (!aes_key || RAND_bytes(aes_key, RSA_KEY_SIZE_BYTES) != 1)
        error_exit("Failed to generate AES key", EXIT_FAILURE);

    BIGNUM *exponent = BN_new();
    // Generating a RSA keypair for the encryption
    printf("Generating a RSA keypair (for the AES key encryption)...\n");
    if (!rsa_keypair || !exponent || !BN_set_word(exponent, RSA_EXPONENT)
        || RSA_generate_key_ex(rsa_keypair, RSA_KEY_SIZE_BITS, exponent, NULL)
            != 1)
        error_exit("Failed to generate RSA keypair", EXIT_FAILURE);

    assert(BN_is_word(exponent, RSA_EXPONENT));
    BN_free(exponent);
}

// Store the RSA modulus and the RSA public exponent in the header
void store_keys_in_headers(struct CryptFS_Header *header, RSA *rsa_keypair,
                           unsigned char *aes_key)
{
    size_t i = 0;
    while (i < NB_ENCRYPTION_KEYS)
    {
        if (header->keys[i].rsa_n == 0 || header->keys[i].rsa_e == 0)
        {
            const BIGNUM *modulus = RSA_get0_n(rsa_keypair);

            if (!modulus
                || !BN_bn2bin(RSA_get0_n(rsa_keypair),
                              (unsigned char *)&header->keys[i].rsa_n))
                error_exit("Failed to store RSA modulus", EXIT_FAILURE);
            header->keys[i].rsa_e = RSA_EXPONENT;

            // Store the encrypted AES key in the header
            printf("Encrypting the AES key with the RSA keypair...\n");
            int encrypted_aes_key_size = RSA_public_encrypt(
                AES_KEY_SIZE_BYTES, aes_key, header->keys[i].aes_key_ciphered,
                rsa_keypair, RSA_PKCS1_OAEP_PADDING);
            if (encrypted_aes_key_size == -1)
            {
                unsigned long errorTrack = ERR_get_error();
                char *error = ERR_error_string(errorTrack, NULL);
                error_exit(error, EXIT_FAILURE);
            }
            break;
        }
    }

    if (i == NB_ENCRYPTION_KEYS)
        error_exit("No more space for any more keys", EXIT_FAILURE);
}

void write_rsa_keys_on_disk(RSA *rsa_keypair, const char *path_to_write)
{
    // Create the paths:
    // "<path_to_write>/.cryptfs"
    // "<path_to_write>/.cryptfs/public.pem"
    // "<path_to_write>/.cryptfs/private.pem"

    char *cryptfs_path =
        xmalloc(strlen(path_to_write) + strlen("/.cryptfs") + 1, 1);
    strcpy(cryptfs_path, path_to_write);
    strcat(cryptfs_path, "/.cryptfs");

    char *public_pem_path =
        xmalloc(strlen(path_to_write) + strlen("/.cryptfs/public.pem") + 1, 1);
    strcpy(public_pem_path, path_to_write);
    strcat(public_pem_path, "/.cryptfs/public.pem");

    char *private_pem_path =
        xmalloc(strlen(path_to_write) + strlen("/.cryptfs/private.pem") + 1, 1);
    strcpy(private_pem_path, path_to_write);
    strcat(private_pem_path, "/.cryptfs/private.pem");

    // Create the directories
    printf("Creating the directories where the RSA keys will be...\n");
    if (mkdir(cryptfs_path, 0755) != 0 && errno != EEXIST)
        error_exit("Failed to create the directories", EXIT_FAILURE);

    // Store the RSA private in ~/.cryptfs/private.pem
    printf("Storing the RSA private key in ~/.cryptfs/private.pem...\n");
    FILE *private_key = fopen(private_pem_path, "w+");
    if (private_key == NULL)
        error_exit("Failed to open the private key file", EXIT_FAILURE);
    if (PEM_write_RSAPrivateKey(private_key, rsa_keypair, NULL, NULL, 0, NULL,
                                NULL)
            != 1
        || fclose(private_key) != 0)
        exit(EXIT_FAILURE);

    // Store the RSA public in ~/.cryptfs/public.pem
    printf("Storing the RSA public key in ~/.cryptfs/public.pem...\n");
    FILE *public_key = fopen(public_pem_path, "w+");
    if (public_key == NULL)
        error_exit("Failed to open the public key file", EXIT_FAILURE);

    if (PEM_write_RSA_PUBKEY(public_key, rsa_keypair) != 1
        || fclose(public_key) != 0)
        error_exit("Failed to write the public key file", EXIT_FAILURE);

    free(cryptfs_path);
    free(public_pem_path);
    free(private_pem_path);
}

int8_t find_rsa_matching_key(RSA *rsa_private, struct CryptFS_Header *header)
{
    for (uint8_t i = 0; i < NB_ENCRYPTION_KEYS; i++)
    {
        // Compare the exponent and the modulus of the both keys
        BIGNUM *header_modulus =
            BN_bin2bn(header->keys[i].rsa_n, RSA_KEY_SIZE_BYTES, NULL);
        if (header->keys[i].rsa_e == RSA_EXPONENT
            && BN_cmp(RSA_get0_n(rsa_private), header_modulus) == 0)
        {
            // Creating test RSA key
            RSA *rsa_test = RSA_new();
            BIGNUM *rsa_test_n =
                BN_bin2bn(header->keys[i].rsa_n, RSA_KEY_SIZE_BYTES, NULL);
            BIGNUM *rsa_test_e =
                BN_lebin2bn((unsigned char *)&header->keys[i].rsa_e,
                            sizeof(uint64_t), NULL);

            BIGNUM *rsa_test_d = BN_dup(RSA_get0_d(rsa_private));

            BIGNUM *rsa_test_p = BN_dup(RSA_get0_p(rsa_private));
            BIGNUM *rsa_test_q = BN_dup(RSA_get0_q(rsa_private));

            if (!rsa_test || !rsa_test_n || !rsa_test_e || !rsa_test_d
                || !rsa_test_p || !rsa_test_q)
                error_exit(
                    "Failed to allocate RSA keypair or to copy the RSA numbers",
                    EXIT_FAILURE);

            if (!RSA_set0_key(rsa_test, rsa_test_n, rsa_test_e, rsa_test_d)
                || !RSA_set0_factors(rsa_test, rsa_test_p, rsa_test_q))
                error_exit("Failed to set the RSA keypair", EXIT_FAILURE);

            if (RSA_check_key(rsa_test) == 1)
            {
                RSA_free(rsa_test);
                BN_free(header_modulus);
                return i;
            }

            // Print RSA error
            unsigned long errorTrack = ERR_get_error();
            char *error = ERR_error_string(errorTrack, NULL);
            fprintf(stderr, "RSA error: %s\n", error);
            RSA_free(rsa_test);
        }
        BN_free(header_modulus);
    }

    return -1;
}

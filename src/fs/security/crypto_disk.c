#include <openssl/evp.h>
#include <openssl/pem.h>

#include "crypto.h"
#include "passphrase.h"
#include "print.h"
#include "xalloc.h"

EVP_PKEY *load_rsa_keypair_from_disk(const char *public_key_path,
                                     const char *private_key_path,
                                     char *passphrase)
{
    EVP_PKEY *rsa_keypair = NULL;

    FILE *public_key_file = fopen(public_key_path, "r");
    FILE *private_key_file = fopen(private_key_path, "r");

    if (public_key_file)
    {
        if (PEM_read_PUBKEY(public_key_file, &rsa_keypair, NULL, NULL) == NULL)
            return NULL;
        fclose(public_key_file);
    }

    if (private_key_file)
    {
        if (PEM_read_PrivateKey(private_key_file, &rsa_keypair, NULL,
                                passphrase)
            == NULL)
            return NULL;
        fclose(private_key_file);
    }

    return rsa_keypair;
}
EVP_PKEY *load_rsa_keypair_from_home(char **passphrase)
{
    // STEP 2 : Verify if the private key is encrypted
    char *home = getenv("HOME");
    if (!home)
        internal_error_exit("Impossible to get the user directory path\n",
                            EXIT_FAILURE);

    char *private_path = xcalloc(PATH_MAX + 1, sizeof(char));
    char *public_path = xcalloc(PATH_MAX + 1, sizeof(char));
    snprintf(private_path, PATH_MAX, "%s/%s", home, ".cryptfs/private.pem");
    snprintf(public_path, PATH_MAX, "%s/%s", home, ".cryptfs/public.pem");

    EVP_PKEY *rsa_keypair =
        load_rsa_keypair_from_disk(public_path, private_path, *passphrase);

    if (rsa_keypair == NULL)
        error_exit("Impossible to load the RSA keypair\n", EXIT_FAILURE);

    free(private_path);
    free(public_path);

    return rsa_keypair;
}

#include <limits.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdio.h>
#include <stdlib.h>

#include "crypto.h"
#include "format.h"
#include "passphrase.h"
#include "print.h"
#include "xalloc.h"

static void ask_new_password(char **passphrase)
{
    printf("Do you want to secure your new key with a passphrase? (y/n): ");
    char answer = getchar();
    getchar(); // Consume the newline

    if (answer != 'y' && answer != 'Y')
    {
        print_warning("No passphrase will be used. The key may be exposed.\n");
        (void)passphrase;
    }
    else
        *passphrase = ask_user_passphrase();
}

int main(int argc, char *argv[])
{
    char *passphrase = NULL;
    char *path = NULL;
    EVP_PKEY *existing_rsa_keypair = NULL;

    switch (argc)
    {
    case 2:
        path = argv[1];

        break;
    case 3:
        path = argv[1];
        passphrase = argv[2];
        break;

    default:
        printf("Usage: %s <file> [RSA key passphrase]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (is_already_formatted(path))
    {
        // Ask the user if he wants to overwrite the file
        print_warning(
            "The file is already formatted. Do you want to overwrite it? "
            "(y/n): ");
        char answer = getchar();
        getchar(); // Consume the newline
        if (answer != 'y' && answer != 'Y')
        {
            printf("Aborting...\n");
            return 0;
        }
        else
            printf("Overwriting...\n");
    }

    // If the keys already exist, ask the user if he wants to use them
    if (keypair_in_home_exist())
    {
        print_info("Keys already generated, do you want to use them? [y/N] ");
        char answer = getchar();
        getchar(); // Remove the newline
        if (answer == 'y' || answer == 'Y')
        {
            // STEP 2 : Verify if the private key is encrypted
            char *home = getenv("HOME");
            if (!home)
                internal_error_exit(
                    "Impossible to get the user directory path\n",
                    EXIT_FAILURE);

            char *private_path = xcalloc(PATH_MAX + 1, sizeof(char));
            char *public_path = xcalloc(PATH_MAX + 1, sizeof(char));
            snprintf(private_path, PATH_MAX, "%s/%s", home,
                     ".cryptfs/private.pem");
            snprintf(public_path, PATH_MAX, "%s/%s", home,
                     ".cryptfs/public.pem");
            if (rsa_private_is_encrypted(private_path))
                passphrase = ask_user_passphrase();

            existing_rsa_keypair = load_rsa_keypair_from_disk(
                public_path, private_path, passphrase);

            if (existing_rsa_keypair == NULL)
                error_exit("Impossible to load the RSA keypair\n",
                           EXIT_FAILURE);

            free(private_path);
            free(public_path);
        }
        else
            ask_new_password(&passphrase);
    }
    else
        ask_new_password(&passphrase);

    format_fs(path, passphrase, existing_rsa_keypair);
    print_success("The file/device `%s` has been formatted successfully!\n",
                  path);

    if (passphrase)
        free(passphrase);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "format.h"
#include "passphrase.h"
#include "print.h"

int main(int argc, char *argv[])
{
    char *passphrase = NULL;
    char *path = NULL;
    switch (argc)
    {
    case 2:
        path = argv[1];

        // Ask for passphrase
        printf("Do you want to secure your key with a passphrase? (y/n): ");
        char answer = getchar();
        getchar(); // Consume the newline
        if (answer != 'y' && answer != 'Y')
            print_warning(
                "No passphrase will be used. The key may be exposed.\n");
        else
            passphrase = ask_user_passphrase();

        break;
    case 3:
        path = argv[1];
        passphrase = argv[2];
        break;

    default:
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
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

    format_fs(path, passphrase);
    free(passphrase);
    print_success("The file/device `%s` has been formatted successfully!\n",
                  path);
    return 0;
}

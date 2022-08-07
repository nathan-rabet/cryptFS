#include <stdio.h>
#include <stdlib.h>

#include "format.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *path = argv[1];
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

    format_fs(path);
    printf("Done!\n");
    return 0;
}

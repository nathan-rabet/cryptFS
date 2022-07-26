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

    format_fs(argv[1]);
    printf("Done!\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

void error_exit(const char *msg, int error_code)
{
    perror(msg);
    exit(error_code);
}

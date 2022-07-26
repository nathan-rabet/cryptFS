#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "format.h"

Test(is_already_formatted, not_formated, .timeout = 10)
{
    cr_assert(!is_already_formatted("tests/format_test.c"));
}

Test(is_already_formatted, formated, .init = cr_redirect_stdout, .timeout = 10)
{
    format_fs("build/format.test.cfs");
    cr_assert(is_already_formatted("build/format.test.cfs"));

    // Detele the file
    if (remove("build/format.test.cfs") != 0)
    {
        perror("Impossible to delete the file");
        exit(EXIT_FAILURE);
    }
}

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <openssl/rand.h>
#include <signal.h>

#include "block.h"
#include "cryptfs.h"
#include "format.h"
#include "xalloc.h"

Test(set_block_size, bad_size, .init = cr_redirect_stderr, .timeout = 10,
     .signal = SIGABRT)
{
    set_block_size(42);
}

Test(set_block_size, good_size, .timeout = 10)
{
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);
}

Test(block, read_write, .init = cr_redirect_stdout, .timeout = 10)
{
    set_device_path("build/block_read_write.test.cfs");
    set_block_size(CRYPTFS_BLOCK_SIZE_BYTES);

    format_fs("build/block_read_write.test.cfs");

    uint8_t *buffer_before = xmalloc(1, get_block_size());
    uint8_t *buffer_after = xcalloc(1, get_block_size());

    cr_assert(RAND_bytes(buffer_before, get_block_size()) == 1);

    int ret = write_blocks(0, 1, buffer_before);
    cr_assert_eq(ret, 0);
    ret = read_blocks(0, 1, buffer_after);
    cr_assert_eq(ret, 0);
    cr_assert_arr_eq(buffer_before, buffer_after, get_block_size());

    // Remove the file
    if (remove("build/block_read_write.test.cfs") != 0)
        cr_assert(false, "Impossible to delete the file");

    free(buffer_before);
    free(buffer_after);
}

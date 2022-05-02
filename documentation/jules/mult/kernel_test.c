#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "second.h"

static __init int hello_init(void)
{
    long unsigned int lenk = strlenk("Module Test.");
    pr_info("multiple: Hello World!\n");
    rec_printk(10);
    printk("multiple: strlenk(\"Module Test.\" = %zu\n", lenk);
    return 0;
}

static __exit void hello_exit(void)
{
    pr_info("multiple: Goodbye.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multiple module");
MODULE_AUTHOR("Jules Aubert");

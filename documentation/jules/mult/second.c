#include <linux/module.h>
#include <linux/printk.h>

long unsigned int strlenk(const char *s)
{
    unsigned int i = 0;
    for (; s[i] != '\0'; ++i);
    return i;
}

void rec_printk(unsigned int i)
{
    if (i == 0)
        return;
    printk("multiple: Test.: rec_printk: %d\n", i);
    rec_printk(i - 1);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multiple module");
MODULE_AUTHOR("Jules Aubert");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "epidriver"

#define EXAMPLE_MSG "EpiDriver\n"
#define MSG_BUFFER_LEN 10

static int major_num;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
static struct file_operations file_ops = {
};

static int __init epidriver_init(void)
{
    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;
    major_num = register_chrdev(0, "epidriver", &file_ops);
    if (major_num < 0)
    {
        pr_alert("Could not register device: %d\n", major_num);
        return major_num;
    }
    else
    {
        pr_info("epidriver: module loaded with device major number %d\n", major_num);
        return 0;
    }
}

static void __exit epidriver_exit(void)
{
    pr_info("epidriver: Goodbye, World!\n");
    unregister_chrdev(major_num, DEVICE_NAME);
}

module_init(epidriver_init);
module_exit(epidriver_exit);

MODULE_LICENSE("Beerware");
MODULE_AUTHOR("Jules Aubert");
MODULE_DESCRIPTION("How to get the major");
MODULE_VERSION("4.2");

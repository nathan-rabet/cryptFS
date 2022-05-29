#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/printk.h>

#define READ_ADDR 0xffffffffa8d748f0

static struct kprobe probe;

int pre_read(struct kprobe* p, struct pt_regs* regs) {
    printk("cryptolib: entered read");
    return 0;
}

void post_read(struct kprobe* p, struct pt_regs* regs, unsigned long flags) {
    printk("cryptolib: exited read");
}

static __init int hello_init(void) {
    pr_info("cryptolib: setting up probe\n");
    void* read_addr = READ_ADDR;
    probe.addr = read_addr;
    probe.pre_handler = pre_read;
    probe.post_handler = post_read;
    register_kprobe(&probe);
    return 0;
}

static __exit void hello_exit(void) {
    unregister_kprobe(&probe);
    pr_info("hello: Goodbye.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Cryptolib");
MODULE_AUTHOR("RABET Nathan");
MODULE_AUTHOR("MOHAMMED BEN ALI Sabir");
MODULE_AUTHOR("REKIK Youness");

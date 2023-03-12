#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AaronComo");
MODULE_DESCRIPTION("A simple Linux module.");
MODULE_VERSION("1.0");

static int __init example_init(void) {
    printk("%s: inserted.\n", __func__);
    printk("Hello world!\n");
    return 0;
}

static void __exit example_exit(void) {
    printk("%s: removed.\n", __func__);
}

module_init(example_init);
module_exit(example_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <asm/uaccess.h>
#include <linux/kprobes.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("AaronComo");
MODULE_DESCRIPTION("Add a new system call to kernel without compiling it.");
MODULE_VERSION("1.0");


#define __NR_my_syscall 335

int orig_cr0;
unsigned long (*kallsyms_lookup_name_func)(const char *) = NULL; /* save the address of unexported kallsyms_lookup_name func */
unsigned long *sys_call_table = NULL;  /* save sys_call_table */
void reset_cr0(unsigned int val);   /* reset guard bit */
unsigned int set_cr0(void);   /* alter guard bit to allow changes of sys_call_table, return cr0's original value */
void find_unexported(void);    /* use kprobe to find unexported kernel function */ 

static int sys_my_syscall(void);    /* declaration of self-defined func */
static int (*orig_syscall)(void);   /* save original syscall of 335 */


unsigned int set_cr0(void) {
    unsigned int cr0 = 0, ret;
    asm volatile ("movq %%cr0, %%rax" : "=a"(cr0)); /* move original value of reg_cr0 to reg_rax and save it in cr0 */
    ret = cr0;
    cr0 &= 0xfffeffff;  /* set 17th bit of cr0 to 0 */
    asm volatile ("movq %%rax, %%cr0" :: "a"(cr0)); /* reg_rax=cr0, reg_cr0=reg_rax */
    return ret;
}

void reset_cr0(unsigned int val) {
    asm volatile ("movq %%rax, %%cr0" :: "a"(val));
}

void find_unexported(void) {
    /**
     * due to security policy, 'kallsyms_lookup_name' is no longer exported after 5.7
     * use kprobe to export 'kallsyms_lookup_name'
     */
    static struct kprobe kp = {
        .symbol_name = "kallsyms_lookup_name"
    };
    int flag = -1;
    flag = register_kprobe(&kp);
    if (flag == -1) {
        printk("fail to find kallsyms_lookup_name.\n");
    }
    if (kp.addr) {
        printk("kallsyms_lookup_name: 0x%p\n", kp.addr);
        kallsyms_lookup_name_func = (void *)kp.addr;
    }
    unregister_kprobe(&kp);
}

static int sys_my_syscall(void) {
    printk("this is a message send from kernel.\n");
    return 335;
}

static int __init append_init(void) {
    printk("%s: inserted.\n", __func__);
    find_unexported();
    sys_call_table = (unsigned long *)(*kallsyms_lookup_name_func)("sys_call_table");
    printk("sys_call_table: 0x%p\n", sys_call_table);
    orig_syscall = (int (*)(void))(sys_call_table[__NR_my_syscall]); /* save address of original syscall */
    orig_cr0 = set_cr0();
    sys_call_table[__NR_my_syscall] = (unsigned long)&sys_my_syscall;
    reset_cr0(orig_cr0);
    return 0;
} 

static void __exit append_exit(void) {
    orig_cr0 = set_cr0();
    sys_call_table[__NR_my_syscall] = (unsigned long)orig_syscall;
    reset_cr0(orig_cr0);
    printk("%s: removed.\n", __func__);
}

module_init(append_init);
module_exit(append_exit);

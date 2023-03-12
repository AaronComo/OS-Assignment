# OS_Assignment

## Assignment 1

### Folders

- module: a simple kernel module.
- syscall: hook `sys_call_table`, in order to append a new syscall without compiling kernel.



### Note

- `kallsyms_lookup_name` was no longer export after version `5.7`
- use `kprobe` to find unexported kernel function. 
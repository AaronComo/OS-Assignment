# OS_Assignment

## Assignment 1

### Folders

- module: a simple kernel module.
- syscall: hook `sys_call_table`, in order to append a new syscall without compiling kernel.

### Note

- `kallsyms_lookup_name` was no longer export after version `5.7`. Therefore we cannot find any kernel function easily.
- To tackle it, we can use `kprobe`, another exported method to find unexported kernel function. 



## Assignment 2

- Implement thread based Fibonacci number calculation program, using `Java` and `C`.
- Encapsulate `<pthread.h>` into `thread.h`. Available APIs: 
  - `int create(void *func);`
  - `void wait(int id);`


## Assignment 3

- Implement dynamic extended thread pool.
- Customize macro `BLOCK_SIZE` to fit your own needs. Default: 64.
- Available APIs: 
  - `int create(void *func);`
  - `void wait(int id);`
- A possible solution to Dining Philosophers problem.

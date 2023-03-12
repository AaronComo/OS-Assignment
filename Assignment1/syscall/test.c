#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main() {
    int ret = syscall(335);
    printf("%d\n", ret);
    return 0;
}

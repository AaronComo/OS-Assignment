#include <stdio.h>
#include "thread.h"

void test(int id) {
    printf("thread id: %d\n", id);
}

int main() {
    for (int i = 0; i < MAX_THREAD; ++i) {
        create(test);
    }
    return 0;
}

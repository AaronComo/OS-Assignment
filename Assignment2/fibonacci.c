#include <stdio.h>
#include "thread.h"

#define MAX_SIZE 1024
static int dp[MAX_SIZE], i, len;
void fibonacci(int);

int main() {
    scanf("%d", &len);
    
    wait(create(fibonacci));
    
    for (i = 1; i < len; i++)
        printf("%d ", dp[i]);
    printf("\n");
    return 0;
}

void fibonacci(int id) {
    printf("thread id: %d\n", id);
    dp[0] = 0;
    dp[1] = 1;
    for (i = 2; i < len; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
}

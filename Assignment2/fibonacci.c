#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 1024
static int dp[MAX_SIZE], i, len;
void *fibonacci(void *);

int main(int argc, char* argv[]) {
    scanf("%d", &len);
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, fibonacci, NULL);
    pthread_join(tid, NULL);
    printf("%d\n", dp[i - 1]);
    return 0;
}

void *fibonacci(void *arg) {
    dp[0] = 0; dp[1] = 1;
    for (i = 2; i < len; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    pthread_exit(NULL);
}

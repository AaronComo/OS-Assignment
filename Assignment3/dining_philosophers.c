#include <stdio.h>
#include "thread.h"


int direction[2] = { 0, 1 };
pthread_mutex_t chopsticks[5];

void pickup_forks(int id) {
    // 2k+1: right first, 2k: left first
    pthread_mutex_lock(&chopsticks[(id + direction[id % 2]) % 5]);
    pthread_mutex_lock(&chopsticks[(id + direction[id % 2 + 1] % 2) % 5]);
    printf("id: %d\teating.\n", id);
}

void return_forks(int id) {
    pthread_mutex_unlock(&chopsticks[(id + direction[id % 2 + 1] % 2) % 5]);
    pthread_mutex_unlock(&chopsticks[(id + direction[id % 2]) % 5]);
    printf("id: %d\tthinking.\n", id);
}

void philosopher(int id) {
    for (int i = 0; i < 2; i++) {
        pickup_forks(id);
        return_forks(id);
    }
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
        create(philosopher);
    }
    return 0;
}

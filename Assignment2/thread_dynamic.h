#ifndef __THREAD_H__
#define __THREAD_H__
#define BLOCK_SIZE 64
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#endif


static int n = 0;
pthread_mutex_t mutex;
enum { T_FREE = 0, T_ALIVE, T_DEAD };
struct thread {
    int id, status;         /* id: index in tpool */
    pthread_t thread;
    pthread_attr_t attr;
    void (*entry)(int);     /* save entry of exec_func */
};

struct thread *tpool, *tptr;

void *__wrapper(void *arg) {
    ((struct thread *)arg)->entry(((struct thread *)arg)->id);
    return NULL;
}

void __tpool_inflate() {
    struct thread *p = tpool;
    p = (struct thread *)calloc(BLOCK_SIZE * (++n), sizeof(struct thread));
    if (tpool) {
        memcpy(p, tpool, (n - 1) * BLOCK_SIZE * sizeof(struct thread));
        free(tpool);
    }
    tpool = p;
    tptr = tpool + (n - 1) * BLOCK_SIZE;
}

int create(void *func) {
    pthread_mutex_lock(&mutex);
    if (tptr - tpool >= n * BLOCK_SIZE) {
        __tpool_inflate();
    }
    *tptr = (struct thread){
        .id = (int)(tptr - tpool),
        .status = T_ALIVE,
        .entry = (void (*)(int))func,
    };
    pthread_attr_init(&(tptr->attr));
    pthread_create(&(tptr->thread), &(tptr->attr), __wrapper, tptr);    /* transfer tptr as params to __wrapper */
    pthread_mutex_unlock(&mutex);
    return (tptr++)->id;
}

void __join() {
    for (int i = 0; i < n * BLOCK_SIZE; ++i) {
        struct thread *t = &tpool[i];
        if (t->status == T_ALIVE) {
            pthread_join(t->thread, NULL);
            t->status = T_DEAD;
        }
    }
}

void wait(int id) {
    pthread_join(tpool[id].thread, NULL);
    tpool[id].status = T_DEAD;
}

__attribute__((constructor)) void init() {
    pthread_mutex_init(&mutex, NULL);
    __tpool_inflate();
}

__attribute__((destructor)) void cleanup() {
    pthread_mutex_destroy(&mutex);
    __join();
}

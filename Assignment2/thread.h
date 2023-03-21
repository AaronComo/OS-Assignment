#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <assert.h>
#endif

#define MAX_THREAD 64
enum { T_FREE = 0, T_ALIVE, T_DEAD };
struct thread {
    int id, status;         /* id: index in tpool */
    pthread_t thread;
    pthread_attr_t attr;
    void (*entry)(int);     /* save entry of exec_func */
};

struct thread tpool[MAX_THREAD], *tptr = tpool;

void *wrapper(void *arg) {
    ((struct thread *)arg)->entry(((struct thread *)arg)->id);
    return NULL;
}

int create(void *func) {
    assert(tptr - tpool < MAX_THREAD);
    *tptr = (struct thread) {
        .id = tptr - tpool,
        .status = T_ALIVE,
        .entry = func,
    };
    pthread_attr_init(&(tptr->attr));
    pthread_create(&(tptr->thread), &(tptr->attr), wrapper, tptr);    /* transfer tptr as params to wrapper */
    return (tptr++)->id;
}

void join() {
    for (int i = 0; i < MAX_THREAD; ++i) {
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

__attribute__((destructor)) void cleanup() {
    join();
}

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

enum {UNLOCKED, LOCKED};

struct Mutex {
    bool lock;
};

void MutexInit(struct Mutex* mutex){
    mutex->lock = UNLOCKED;
}

void MutexAcquire(struct Mutex* mutex){
    while(!__sync_bool_compare_and_swap(&mutex->lock, UNLOCKED, LOCKED))
        usleep(1);
}

void MutexRelease(struct Mutex* mutex){
    mutex->lock=UNLOCKED;
    __sync_synchronize();
}

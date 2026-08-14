#ifndef MMLOCK_H
#define MMLOCK_H

#include "MMTypes.h"

#include <pthread.h>

typedef struct MMLock {
    int type;
    int retainCount;
    //-------------
    pthread_mutex_t mutex;
} MMLock;

MMLock * MMLock_init();
void MMLock_lock(MMLock * recv);
void MMLock_unlock(MMLock * recv);

#endif /*MMLOCK_H*/

#ifndef MMOPERATIONQUEUE_H
#define MMOPERATIONQUEUE_H

#include "MMinvocationOperation.h"
#include "MMArray.h"

#include <pthread.h>

typedef struct MMOperationQueue {
    int type;
    int retainCount;
    //-------------
    MMMutableArray * operations;
    // sincronization
    int runningCount; 
#if defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE condition;
#else
    pthread_mutex_t lock;
    pthread_cond_t condition;
#endif
} MMOperationQueue;

MMOperationQueue *MMOperationQueue_init();
void MMOperationQueue_addOperation(MMOperationQueue * recv,  MMInvocationOperation *op);
void MMOperationQueue_waitUntilAllOperationsAreFinished(MMOperationQueue *recv);

MMOperationQueue *MMOperationQueue_copy(MMOperationQueue * recv);

#endif 
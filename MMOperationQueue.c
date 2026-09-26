#include "MMOperationQueue.h"
#include "MMTypes.h"

#if defined(_WIN32) || defined(_WIN64)
static DWORD WINAPI operation_thread(LPVOID arg)
#else
static void *operation_thread(void *arg)
#endif
{
    typedef struct {
        MMInvocationOperation *op;
        MMOperationQueue *queue;
    } ThreadContext;

    ThreadContext *ctx = (ThreadContext *)arg;
    MMInvocationOperation *op = ctx->op;
    MMOperationQueue *queue = ctx->queue;

    // Start the job
    if (op->func) {
        op->func(op->context);
    }

    // Mark as finished and notify
#if defined(_WIN32) || defined(_WIN64)
    EnterCriticalSection(&queue->lock);
#else
    pthread_mutex_lock(&queue->lock);
#endif

    op->isFinished = 1;
    queue->runningCount--;

#if defined(_WIN32) || defined(_WIN64)
    LeaveCriticalSection(&queue->lock);
    WakeConditionVariable(&queue->condition);
#else
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_signal(&queue->condition);
#endif

    free(ctx);

#if defined(_WIN32) || defined(_WIN64)
    return 0;
#else
    return NULL;
#endif
}

MMOperationQueue *MMOperationQueue_init(){
    MMOperationQueue * queue = MM_init(MMTypeOperationQueue);
    queue->operations = MMMutableArray_initWithCapacity(0);
    queue->runningCount = 0;

#if defined(_WIN32) || defined(_WIN64)
    InitializeCriticalSection(&queue->lock);
    InitializeConditionVariable(&queue->condition);
#else
    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->condition, NULL);
#endif
    
    return queue;
}

void MMOperationQueue_addOperation(MMOperationQueue * recv,  MMInvocationOperation * op) {

#if defined(_WIN32) || defined(_WIN64)
    EnterCriticalSection(&recv->lock);
#else
    pthread_mutex_lock(&recv->lock);
#endif

    MMMutableArray_addObject(recv->operations, op);
    recv->runningCount++;

#if defined(_WIN32) || defined(_WIN64)
    LeaveCriticalSection(&recv->lock);
#else
    pthread_mutex_unlock(&recv->lock);
#endif

    // Context preparation
    typedef struct {
        MMInvocationOperation *op;
        MMOperationQueue *queue;
    } ThreadContext;

    ThreadContext *ctx = malloc(sizeof(ThreadContext));
    ctx->op = op;
    ctx->queue = recv;

#if defined(_WIN32) || defined(_WIN64)
    HANDLE h = CreateThread(NULL, 0, operation_thread, ctx, 0, NULL);
    if (h) CloseHandle(h);
    else {
        free(ctx);
    }
#else
    pthread_t thread;

    //printf("ADD op=%p func=%p running=%d\n", op, (void*)op->func, recv->runningCount);
    if (pthread_create(&thread, NULL, operation_thread, ctx) == 0) {
        pthread_detach(thread);
    } else {
        free(ctx);
    }
#endif
}

void MMOperationQueue_waitUntilAllOperationsAreFinished(MMOperationQueue *recv)
{
    if (!recv) return;

#if defined(_WIN32) || defined(_WIN64)
    EnterCriticalSection(&recv->lock);
    while (recv->runningCount > 0) {
        SleepConditionVariableCS(&recv->condition, &recv->lock, INFINITE);
    }
    LeaveCriticalSection(&recv->lock);
#else
    pthread_mutex_lock(&recv->lock);
    while (recv->runningCount > 0) {
        pthread_cond_wait(&recv->condition, &recv->lock);
    }
    pthread_mutex_unlock(&recv->lock);
#endif
}

MMOperationQueue *MMOperationQueue_copy(MMOperationQueue * recv){
    MMOperationQueue * n = MM_init(MMTypeOperationQueue);
    n->operations = MMMutableArray_copy(recv->operations);
    return n;
}

void MMOperationQueue_release(MMOperationQueue *recv)
{
    if (!recv) return;

    MMOperationQueue_waitUntilAllOperationsAreFinished(recv);

    // Release all operations
    if (recv->operations) {
        MM_release(recv->operations);
        recv->operations = NULL;
    }
#if defined(_WIN32) || defined(_WIN64)
    DeleteCriticalSection(&recv->lock);
    // CONDITION_VARIABLE not requires explicit destruction
#else
    pthread_mutex_destroy(&recv->lock);
    pthread_cond_destroy(&recv->condition);
#endif
    free(recv);
}
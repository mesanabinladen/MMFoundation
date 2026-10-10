
#ifndef MMINVOCATIONOPERATION_H
#define MMINVOCATIONOPERATION_H

typedef void (*SEL)(void *context);

typedef struct {
    int type;
    int retainCount;
    //-------------
    SEL func;
    void *context;
    int isFinished;
} MMInvocationOperation;

MMInvocationOperation * MMInvocationOperation_init();
MMInvocationOperation * MMInvocationOperation_initWithTarget(SEL sel, void * arg);

#endif
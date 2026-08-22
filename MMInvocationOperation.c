#include "MMinvocationOperation.h"
#include "MMTypes.h"

MMInvocationOperation * MMinvocationOperation_init(){
    MMInvocationOperation * n = MM_init(MMTypeInvocationOperation);
    return n;
}

MMInvocationOperation * MMinvocationOperation_initWithTarget(SEL sel, void * arg){
    MMInvocationOperation *op = MMinvocationOperation_init();
    if (!op) return nil;

    op->func = sel;
    op->context = arg;
    return op;
}

void MMInvocationOperation_release(MMInvocationOperation * recv){
    if (!recv) return;
    free(recv);
}
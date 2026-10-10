#include "MMInvocationOperation.h"
#include "MMTypes.h"

MMInvocationOperation * MMInvocationOperation_init(){
    MMInvocationOperation * n = MM_init(MMTypeInvocationOperation);
    return n;
}

MMInvocationOperation * MMInvocationOperation_initWithTarget(SEL sel, void * arg){
    MMInvocationOperation *op = MMInvocationOperation_init();
    if (!op) return nil;

    op->func = sel;
    op->context = arg;
    return op;
}

void MMInvocationOperation_release(MMInvocationOperation * recv){
    if (!recv) return;
    free(recv);
}
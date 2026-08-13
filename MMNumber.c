#include "MMNumber.h"
#include "MMTypes.h"

MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->ui = value;
    return n;
}

void MMNumber_release(MMNumber * recv){
    if (!recv) return;
    free(recv);
    recv = nil;
}

#include "MMNumber.h"

#include <stdlib.h>

#include "MMTypes.h"

MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value){
    MMNumber * n = malloc(sizeof(MMNumber *));
    n->type=MMTypeNumber;
    n->retainCount=1;
    n->ui = value;
    return n;
}

void MMNumber_release(MMNumber * n){
    if (!n) return;
    free(n);
    n = nil;
}

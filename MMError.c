#include "MMError.h"

#include "MMString.h"

MMError *MMError_init(){
    MMError * n = MM_init(MMTypeError);
    return n;
}

MMError *MMError_copy(MMError *recv){
    MMError * new = MMError_init();
    new->code=recv->code;
    new->domain=MMString_copy(recv->domain);
    new->localizedDescription=MMString_copy(recv->localizedDescription);
    return new;
}

void MMError_release(MMError * recv){
    if (!recv) return;
    if (recv->domain) MM_release(recv->domain);   
    if (recv->localizedDescription) MM_release(recv->localizedDescription);
    free(recv);
}
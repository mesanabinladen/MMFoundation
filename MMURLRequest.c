#include "MMURLRequest.h"

#
MMURLRequest * MMURLRequest_init(){
    MMURLRequest * n = MM_init(MMTypeNumber);
    return n;
}

MMURLRequest * MMURLRequest_requestWithURL(MMURL * URL){
    MMURLRequest *new=MMURLRequest_init();
    new->URL = MMURL_copy(URL);
    return new;
}

MMURLRequest *MMURLRequest_copy(MMURLRequest *recv){
    return MMURLRequest_requestWithURL(recv->URL);
}

void MMURLRequest_release(MMURLRequest *recv){
    if (!recv) return;
    free(recv);
}
#include "MMHTTPURLResponse.h"


MMHTTPURLResponse *MMHTTPURLResponse_init(){
    MMHTTPURLResponse * n = MM_init(MMTypeHTTPURLResponse);
    return n;
}

MMHTTPURLResponse *MMTTPURLResponse_copy(MMHTTPURLResponse *recv){
    MMHTTPURLResponse *n=MMHTTPURLResponse_init();
    n->statusCode=recv->statusCode;
    n->url=MMURL_copy(recv->url);
    return n;
}

void MMHTTPURLResponse_release(MMHTTPURLResponse *recv){
    if (!recv) return;
    MM_release(recv->url);
    free(recv);
}
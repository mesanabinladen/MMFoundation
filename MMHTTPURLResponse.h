#ifndef MMHTTPURLRESPONSE_H
#define MMHTTPURLRESPONSE_H

#include "MMTypes.h"
#include "MMURL.h"

typedef struct MMHTTPURLResponse {
    int type;
    int retainCount;
    //-------------
    MMInteger statusCode;
    MMURL *url;
} MMHTTPURLResponse;


MMHTTPURLResponse *MMHTTPURLResponse_init();
MMHTTPURLResponse *MMTTPURLResponse_copy(MMHTTPURLResponse *recv);

#endif /*MMHTTPURLRESPONSE_H*/
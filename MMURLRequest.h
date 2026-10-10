#ifndef MMURLREQUEST_H
#define MMURLREQUEST_H

#include "MMTypes.h"

#include "MMURL.h"

typedef struct MMURLRequest {
    int type;
    int retainCount;
    //-------------
    MMURL * URL;
} MMURLRequest;

MMURLRequest *MMURLRequest_init();
MMURLRequest *MMURLRequest_requestWithURL(MMURL * URL);

#endif /*MMURLREQUEST_H*/
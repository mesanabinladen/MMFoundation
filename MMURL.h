#ifndef MMURL_H
#define MMURL_H

#include "MMTypes.h"

typedef struct MMURL {
    int type;
    int retainCount;
    //-------------
    MMString * url;
} MMURL;

MMURL *MMURL_init();
MMURL *MMURL_initWithString(MMString * URLString);
MMURL *MMURL_initWithCString(char * URLString);

#endif /*MMURL_H*/
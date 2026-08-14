#include "MMURL.h"

#include "MMString.h"

MMURL *MMURL_init(){
    MMURL * n = MM_init(MMTypeURL);
    return n;
}

MMURL *MMURL_initWithString(MMString * URLString){
    if (!URLString) return nil;

    MMURL *newUrl = MMURL_init();
    if (!newUrl) return nil;
    newUrl->url = MMString_copy(URLString);
    return newUrl;
}

MMURL *MMURL_initWithCString(char * URLString){
    if (!URLString) return nil;
    MMString *s=MMString_initWithCString(URLString);
    MMURL * new = MMURL_initWithString(s);
    MM_release(s);
    return new;
}

MMURL *MMURL_copy(MMURL *recv){
    return MMURL_initWithString(recv->url);
}

void MMURL_release(MMURL *recv){
    if (!recv) return;
    MM_release(recv->url);
    free(recv);
}
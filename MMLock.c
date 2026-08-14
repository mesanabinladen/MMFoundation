#include "MMLock.h"

MMLock * MMLock_init(){
    MMLock * n = MM_init(MMTypeLock);
    return n;
}

void MMLock_lock(MMLock * recv){
    if(!recv)return;
    pthread_mutex_lock(&recv->mutex);
}

void MMLock_unlock(MMLock * recv){
    if(!recv)return;
    pthread_mutex_unlock(&recv->mutex);
}

void MMLock_release(MMLock * recv){
    if (!recv) return;
    pthread_mutex_destroy(&recv->mutex);//must be unlocked!
    free(recv);
}
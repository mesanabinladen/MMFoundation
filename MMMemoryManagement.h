#ifndef MMMEMORYMANAGEMENT_H
#define MMMEMORYMANAGEMENT_H

void * MM_init(int type);
void MM_release(void* anObject);

void MMAutoreleasePool_init();
void MMAutoreleasePool_drain();

#endif /* MMMEMORYMANAGEMENT_H */
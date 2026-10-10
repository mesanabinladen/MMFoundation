#ifndef MMVALUE_H
#define MMVALUE_H

#include "MMTypes.h"

typedef struct MMValue {
    int type;
    int retainCount;
    //-------------
    void *bytes;
    size_t size;
} MMValue;

MMValue *MMValue_valueWithBytes(const void *value, size_t size);
MMValue *MMValue_valueWithPointer(const void *pointer);
MMBool MMValue_getValue(const MMValue *recv, void *buffer, size_t bufferSize);
void *MMValue_pointerValue(const MMValue *recv);

#endif /* MMVALUE_H */

#ifndef MMARRAY_H
#define MMARRAY_H

#include <stddef.h>
#include <stdarg.h>
#include "MMTypes.h"

typedef struct MMArray {
    int type;
    int retainCount;
    //-------------
    void **items;
    size_t count;
    size_t capacity;
} MMArray;

MMArray *MMArray_init(void);
MMArray *MMArray_initWithObjects(void *first, ...);
MMArray *MMArray_initWithCStringsArray(void *first, ...);
MMArray *MMArray_initWithArray(MMArray * array, MMBool flag);
size_t MMArray_count(const MMArray *recv);
void *MMArray_objectAtIndex(const MMArray *recv, size_t index);
MMArray * MMArray_copy(const MMArray * recv);

//MUTABLE ARRAY
typedef MMArray MMMutableArray;

MMMutableArray *MMMutableArray_init(void);
MMMutableArray *MMMutableArray_initWithCapacity(size_t length);
MMMutableArray *MMMutableArray_initWithObjects(void *first, ...);
MMMutableArray *MMMutableArray_initWithCStringsArray(void *first, ...);
MMMutableArray *MMMutableArray_initWithArray(MMArray * array, MMBool flag);
size_t MMMutableArray_count(const MMMutableArray *recv);
void * MMMutableArray_objectAtIndex(const MMMutableArray *recv, size_t index);
void MMMutableArray_addObject(MMMutableArray * recv, void * anObject);
void MMMutableArray_replaceObjectAtIndex(const MMMutableArray *recv, MMUInteger index, void* anObject);
void MMMutableArray_removeObjectAtIndex(MMMutableArray * recv, MMUInteger index);
void MMMutableArray_removeLastObject(MMMutableArray * recv);
MMMutableArray * MMMutableArray_copy(const MMMutableArray * recv);

#endif /*MMARRAY_H*/

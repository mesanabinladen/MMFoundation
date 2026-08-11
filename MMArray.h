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

/*Create an empty MMArray */
MMArray *MMArray_init(void);

/* Create an MMArray with a NULL-terminated list of objects (variadic) */
MMArray *MMArray_initWithObjects(void *first, ...);

MMArray *MMArray_initWithCStringsArray(void *first, ...);

/* Return number of objects */
size_t MMArray_count(const MMArray *arr);

/* Return object at index or NULL if out-of-range */
void *MMArray_objectAtIndex(const MMArray *arr, size_t index);

/* Free the MMArray (does not free contained objects) */
void MMArray_release(MMArray *arr);

//MUTABLE ARRAY
typedef MMArray MMMutableArray;

/*Create an empty MMMutableArray */
MMMutableArray *MMMutableArray_init(void);

MMMutableArray *MMMutableArray_initWithCapacity(size_t length);

/* Create an MMMutableArray with a NULL-terminated list of objects (variadic) */
MMMutableArray *MMMutableArray_initWithObjects(void *first, ...);

MMMutableArray *MMMutableArrayinitWithCStringsArray(void *first, ...);

/* Return number of objects */
size_t MMMutableArray_count(const MMMutableArray *arr);

/* Return object at index or NULL if out-of-range */
void * MMMutableArray_objectAtIndex(const MMMutableArray *arr, size_t index);

void MMMutableArray_addObject(MMMutableArray * arr, void * anObject);

/* Free the MMMutableArray (does not free contained objects) */
void MMMutableArray_release(MMMutableArray *arr);

#endif /* MMARRAY_H */

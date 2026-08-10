#ifndef MMARRAY_H
#define MMARRAY_H

#include <stddef.h>

typedef struct MMArray {
    void **items;
    size_t count;
    size_t capacity;
} MMArray;

/*Create an empty MMArray */
MMArray *MMArray_init(void);

/* Create an MMArray with a NULL-terminated list of objects (variadic) */
MMArray *MMArray_initWithObjects(void *first, ...);

/* Alternative: create from an array of pointers */
MMArray *MMArray_initWithObjectsArray(void **objects, size_t count);

/* Return number of objects */
size_t MMArray_count(const MMArray *arr);

/* Return object at index or NULL if out-of-range */
void *MMArray_objectAtIndex(const MMArray *arr, size_t index);

/* Free the MMArray (does not free contained objects) */
void MMArray_release(MMArray *arr);

#endif /* MMARRAY_H */

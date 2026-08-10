#include "MMArray.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static int MMArray_ensure_capacity(MMArray *arr, size_t minCapacity) {
    if (arr->capacity >= minCapacity) return 0;
    size_t newCap = arr->capacity ? arr->capacity * 2 : 4;
    while (newCap < minCapacity) newCap *= 2;
    void **newItems = (void**)realloc(arr->items, newCap * sizeof(void*));
    if (!newItems) return -1;
    arr->items = newItems;
    arr->capacity = newCap;
    return 0;
}

/*Create an empty MMArray */
MMArray *MMArray_init(void){
    MMArray *arr = (MMArray*)malloc(sizeof(MMArray));
    if (!arr) return NULL;
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
    return arr;
}

MMArray *MMArray_initWithObjects(void *first, ...) {
    MMArray *arr = (MMArray*)malloc(sizeof(MMArray));
    if (!arr) return NULL;
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;

    va_list ap;
    va_start(ap, first);
    void *obj = first;
    while (obj) {
        if (MMArray_ensure_capacity(arr, arr->count + 1) != 0) {
            va_end(ap);
            MMArray_release(arr);
            return NULL;
        }
        arr->items[arr->count++] = obj;
        obj = va_arg(ap, void*);
    }
    va_end(ap);
    return arr;
}

MMArray *MMArray_initWithObjectsArray(void **objects, size_t count) {
    MMArray *arr = (MMArray*)malloc(sizeof(MMArray));
    if (!arr) return NULL;
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
    if (count > 0) {
        if (MMArray_ensure_capacity(arr, count) != 0) {
            free(arr);
            return NULL;
        }
        memcpy(arr->items, objects, count * sizeof(void*));
        arr->count = count;
    }
    return arr;
}

size_t MMArray_count(const MMArray *arr) {
    return arr ? arr->count : 0;
}

void *MMArray_objectAtIndex(const MMArray *arr, size_t index) {
    if (!arr) return NULL;
    if (index >= arr->count) return NULL;
    return arr->items[index];
}

//release
void MMArray_release(MMArray *arr) {
    if (!arr) return;
    free(arr->items);
    free(arr);
}

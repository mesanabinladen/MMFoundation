#include "MMArray.h"
#include "MMTypes.h"
#include "MMData.h"
#include "MMDate.h"
#include "MMFileHandle.h"
#include "MMString.h"
#include "MMMemoryManagement.h"

MMArray *_initWithObjectsVaList(void *first, va_list ap) {

    va_list copy;
    va_copy(copy, ap);

    // count arguments first
    void *obj = first;
    int numObj = 0;
    while (obj) {
        numObj++;
        obj = va_arg(copy, void*);
    }
    va_end(copy);

    MMMutableArray *arr = MMMutableArray_initWithCapacity(numObj);
    if (!arr) return nil;

    va_copy(copy, ap);
    obj = first;
    numObj = 0;
    while (obj) {
        MM_retain(obj);
        arr->items[numObj] = obj;
        numObj++;
        obj = va_arg(copy, void*);
    }
    va_end(copy);
    arr->count = numObj;

    return (MMArray *)arr;
}

MMArray *_initWithCStringsVaList(void *first, va_list ap) {

    va_list copy;
    va_copy(copy, ap);

    // count arguments first
    void *obj = first;
    int numObj = 0;
    while (obj) {
        numObj++;
        obj = va_arg(copy, void*);
    }
    va_end(copy);

    MMMutableArray *arr = MMMutableArray_initWithCapacity(numObj);
    if (!arr) return nil;

    va_copy(copy, ap);
    obj = first;
    numObj = 0;
    while (obj) {
        MMString * str = MMString_initWithCString(obj);
        arr->items[numObj] = str;
        //I won't increase retain count beacuse it's already 1
        numObj++;
        obj = va_arg(copy, void*);
    }
    va_end(copy);
    arr->count = numObj;

    return (MMArray *)arr;
}

/*Create an empty MMArray */
MMArray *MMArray_init(void){
    MMArray *arr = MM_init(MMTypeArray);

    arr->items = nil;
    arr->count = 0;
    return arr;
}

MMArray *MMArray_initWithObjects(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithObjectsVaList(first, ap);
    va_end(ap);
    return arr;
}

MMArray *MMArray_initWithCStringsArray(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithCStringsVaList(first, ap);
    va_end(ap);
    return arr;
}

size_t MMArray_count(const MMArray *arr) {
    return arr ? arr->count : 0;
}

void *MMArray_objectAtIndex(const MMArray *arr, size_t index) {
    if (!arr) return nil;
    if (index >= arr->count) return nil;
    return arr->items[index];
}

//release
void MMArray_release(MMArray *arr) {
    if (!arr) return;

    for (int i=0; i<arr->count;i++)
    {   
        if (arr->items[i]) {
            MM_release(arr->items[i]);
            arr->items[i] = nil;
        }
    }
    free(arr->items);
    free(arr);
    arr = nil;
}

/*Create an empty MMMutableArray */
MMMutableArray *MMMutableArray_init(void){
    return (MMMutableArray *) MMArray_init();
}

MMMutableArray *MMMutableArray_initWithCapacity(size_t length){
    MMMutableArray *arr = MM_init(MMTypeMutableArray);

    arr->items = malloc(length*sizeof(void *));
    if (!arr->items ) {
        return nil;
    }

    arr->capacity = length;
    arr->count = 0;
    return arr;
}

MMMutableArray *MMMutableArray_initWithObjects(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithObjectsVaList(first, ap);
    va_end(ap);
    return (MMMutableArray *)arr;
}

MMMutableArray *MMMutableArray_initWithCStringsArray(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithCStringsVaList(first, ap);
    va_end(ap);
    return (MMMutableArray *)arr;
}

size_t MMMutableArray_count(const MMMutableArray *arr) {
    return MMArray_count((MMArray *)arr);
}

void *MMMutableArray_objectAtIndex(const MMMutableArray *arr, size_t index) {
    return MMArray_objectAtIndex(arr, index);
}

void MMMutableArray_addObject(MMMutableArray * arr, void * anObject){
    size_t newCount = arr->count + 1;
    void **newItems = realloc(arr->items, newCount * sizeof(void*));
    if (!newItems){
        MMArray_release(arr);
        printf("Error adding object to array\n");
    }
    ((ObjectType *)anObject)->retainCount++;
    arr->items = newItems;
    arr->items[arr->count] = anObject;
    arr->count = newCount;
}

//release
void MMMutableArray_release(MMMutableArray *arr) {
    MMArray_release(arr);
}


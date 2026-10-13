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
    size_t numObj = 0;
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
    size_t numObj = 0;
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
    if (!arr) return nil;

    arr->items = nil;
    arr->count = 0;
    arr->capacity = 0;
    return arr;
}

MMArray *MMArray_initWithObjects(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithObjectsVaList(first, ap);
    va_end(ap);
    if (arr) arr->type = MMTypeArray;
    return arr;
}

MMArray *MMArray_initWithCStringsArray(void *first, ...) {
    va_list ap;
    va_start(ap, first);
    MMArray *arr = _initWithCStringsVaList(first, ap);
    va_end(ap);
    if (arr) arr->type = MMTypeArray;
    return arr;
}

MMArray *MMArray_initWithArray(MMArray * array, MMBool flag){
    if (!array) return nil;

    MMArray *newArr = MM_init(MMTypeArray);
    if (!newArr) return nil;

    newArr->items = nil;
    newArr->count = 0;
    newArr->capacity = 0;

    if (array->count > SIZE_MAX / sizeof(void *)) {
        MM_release(newArr);
        return nil;
    }
    if (array->count > 0) {
        newArr->items = malloc(array->count * sizeof(void *));
        if (!newArr->items) {
            MM_release(newArr);
            return nil;
        }
    }
    newArr->count = array->count;
    newArr->capacity = array->count;
    
    if (flag){
        for (size_t i=0; i<newArr->count; i++){
            newArr->items[i] = MM_copy(array->items[i]); //each object in array receives a MM_copy() message to create a copy of the object. In a managed memory environment, this is instead of the retain message the object would otherwise receive. The object copy is then added to the returned array.
        }
    }
    else{
        if (newArr->count > 0) {
            memcpy(newArr->items, array->items, newArr->count*sizeof(void *));
        }
        for (size_t i=0; i<newArr->count; i++){
            MM_retain(newArr->items[i]); //each object in array simply receives a retain message when it is added to the returned array.
        }
    }

    return newArr;
}

size_t MMArray_count(const MMArray *recv) {
    return recv ? recv->count : 0;
}

void *MMArray_objectAtIndex(const MMArray *recv, size_t index) {
    if (!recv) return nil;
    if (index >= recv->count) return nil;
    return recv->items[index];
}

MMArray * MMArray_copy(const MMArray * recv){
    if (!recv) return nil;

    MMArray *newPtr = malloc(sizeof(*newPtr));
    if (!newPtr) return nil;
    memcpy(newPtr, recv, sizeof(*newPtr));
    newPtr->retainCount = 1;
    newPtr->items = nil;
    newPtr->capacity = newPtr->count;

    if (newPtr->count > SIZE_MAX / sizeof(void *)) {
        free(newPtr);
        return nil;
    }
    if (newPtr->count > 0) {
        newPtr->items = malloc(newPtr->count * sizeof(void *));
        if (!newPtr->items) {
            free(newPtr);
            return nil;
        }
    }

    //copy every single contained object
    for (size_t i=0; i<newPtr->count;i++) {   
        newPtr->items[i] = MM_copy(recv->items[i]);
    }

    return newPtr;
}

//release
void MMArray_release(MMArray *recv) {
    if (!recv) return;

    if (recv->items) {
        for (size_t i=0; i<recv->count;i++) {
            if (!recv->items[i]) continue;
            MM_release(recv->items[i]);
            recv->items[i] = nil;
        }
    }
    free(recv->items);
    free(recv);
}

MMMutableArray *MMMutableArray_init(void){
    MMMutableArray *arr = MM_init(MMTypeMutableArray);
    if (!arr) return nil;
    arr->items = nil;
    arr->count = 0;
    arr->capacity = 0;
    return arr;
}

MMMutableArray *MMMutableArray_initWithCapacity(size_t length){
    if (length > SIZE_MAX / sizeof(void *)) return nil;

    MMMutableArray *arr = MM_init(MMTypeMutableArray);
    if (!arr) return nil;

    arr->items = nil;
    arr->capacity = 0;
    arr->count = 0;
    if (length > 0) {
        arr->items = malloc(length * sizeof(void *));
    }
    if (length > 0 && !arr->items) {
        MM_release(arr);
        return nil;
    }

    arr->capacity = length;
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

MMMutableArray *MMMutableArray_initWithArray(MMArray * array, MMBool flag){
    MMMutableArray *newArr = (MMMutableArray *)MMArray_initWithArray(array, flag);
    if (newArr) newArr->type = MMTypeMutableArray;
    return newArr;
}
size_t MMMutableArray_count(const MMMutableArray *recv) {
    return MMArray_count((MMArray *)recv);
}

void *MMMutableArray_objectAtIndex(const MMMutableArray *recv, size_t index) {
    return MMArray_objectAtIndex(recv, index);
}

void MMMutableArray_addObject(MMMutableArray * recv, void * anObject){
    if (!recv || !anObject) return;
    size_t maxCapacity = SIZE_MAX / sizeof(void *);
    if (recv->count >= maxCapacity) {
        printf("Error adding object to array\n");
        return;
    }

    size_t newCount = recv->count + 1;
    if (newCount > recv->capacity) {
        size_t newCapacity = recv->capacity == 0 ? (maxCapacity < 4 ? maxCapacity : 4) : recv->capacity;
        while (newCapacity < newCount) {
            if (newCapacity > maxCapacity / 2) {
                newCapacity = maxCapacity;
                break;
            }
            newCapacity *= 2;
        }

        void **newItems = realloc(recv->items, newCapacity * sizeof(void *));
        if (!newItems) {
            printf("Error adding object to array\n");
            return;
        }
        recv->items = newItems;
        recv->capacity = newCapacity;
    }

    MM_retain(anObject);
    recv->items[recv->count] = anObject;
    recv->count = newCount;
}

void MMMutableArray_replaceObjectAtIndex(const MMMutableArray *recv, MMUInteger index, void* anObject){
    if (!recv || !anObject) return;
    if (index >= recv->count) return;
    
    MM_retain(anObject);
    MM_release(recv->items[index]);
    recv->items[index] = anObject;
}

void MMMutableArray_removeObjectAtIndex(MMMutableArray * recv, MMUInteger index){
    if (!recv || index >= recv->count) return;

    MM_release(MMArray_objectAtIndex(recv, index));
    
    for (size_t n=index+1; n< recv->count; n++)
    {
        recv->items[n-1]=recv->items[n];
    }
    size_t newCount = recv->count - 1;
    if (newCount == 0) {
        free(recv->items);
        recv->items=nil;
        recv->capacity = 0;
    } else {
        recv->items[newCount] = nil;
    }
    
    recv->count = newCount;
}

void MMMutableArray_removeLastObject(MMMutableArray * recv){
    if (!recv || recv->count == 0) return;
    MMMutableArray_removeObjectAtIndex(recv, recv->count-1);
}

void MMMutableArray_removeAllObjects(MMMutableArray * recv){
    if (!recv) return;
    for (size_t n = 0; n < recv->count; n++) {
        MM_release(recv->items[n]);
    }
    free(recv->items);
    recv->items = nil;
    recv->count = 0;
    recv->capacity = 0;
}

MMMutableArray * MMMutableArray_copy(const MMMutableArray * recv){
    return (MMMutableArray *) MMArray_copy(recv);
}

void MMMutableArray_release(MMMutableArray *recv) {
    MMArray_release(recv);
}

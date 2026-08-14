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

MMArray *MMArray_initWithArray(MMArray * array, MMBool flag){
    MMArray *newArr = MM_init(MMTypeArray);

    newArr->count = array->count;
    newArr->items = malloc(newArr->count*sizeof(void *));
    
    if (flag){
        for (size_t i=0; i<newArr->count; i++){
            newArr->items[i] = MM_copy(array->items[i]); //each object in array receives a MM_copy() message to create a copy of the object. In a managed memory environment, this is instead of the retain message the object would otherwise receive. The object copy is then added to the returned array.
        }
    }
    else{
        memcpy(&newArr->items, &array->items, newArr->count);
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
    ObjectType * ptr = (ObjectType *)recv;
    MMArray * newPtr = nil;

    //allocate main struct
    size_t objSize = sizeof(MMArray);;
    newPtr = malloc(sizeof(objSize));
    memcpy(newPtr, ptr, objSize);
    newPtr->retainCount = 1;

    //copy every single contained object
    for (size_t i=0; i<newPtr->count;i++) {   
        newPtr->items[i] = MM_copy(recv->items[i]);
    }

    return newPtr;
}

//release
void MMArray_release(MMArray *recv) {
    if (!recv) return;

    for (size_t i=0; i<recv->count;i++)
    {   
        if (recv->items[i]) {
            MM_release(recv->items[i]);
            recv->items[i] = nil;
        }
    }
    free(recv->items);
    free(recv);
}

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

MMMutableArray *MMMutableArray_initWithArray(MMArray * array, MMBool flag){
    return (MMMutableArray *)MMArray_initWithArray(array, flag);
}
size_t MMMutableArray_count(const MMMutableArray *recv) {
    return MMArray_count((MMArray *)recv);
}

void *MMMutableArray_objectAtIndex(const MMMutableArray *recv, size_t index) {
    return MMArray_objectAtIndex(recv, index);
}

void MMMutableArray_addObject(MMMutableArray * recv, void * anObject){
    size_t newCount = recv->count + 1;
    void **newItems = realloc(recv->items, newCount * sizeof(void*));
    if (!newItems){
        MMArray_release(recv);
        printf("Error adding object to array\n");
    }
    ((ObjectType *)anObject)->retainCount++;
    recv->items = newItems;
    recv->items[recv->count] = anObject;
    recv->count = newCount;
}

void MMMutableArray_replaceObjectAtIndex(const MMMutableArray *recv, MMUInteger index, void* anObject){
    if (!recv || index < 0 || !anObject) return;
    if (index> recv->count) return;
    
    //replace object
    MM_release(recv->items[index]);
    recv->items[index] = anObject;
    MM_retain(recv->items[index]);
}

void MMMutableArray_removeObjectAtIndex(MMMutableArray * recv, MMUInteger index){
    if (!recv || index < 0 || index >= recv->count) return;

    MM_release(MMArray_objectAtIndex(recv, index));
    
    for (size_t n=index+1; n< recv->count; n++)
    {
        recv->items[n-1]=recv->items[n];
    }

    size_t newCount = recv->count - 1;
    recv->items = realloc(recv->items, newCount * sizeof(void*));
    recv->count = newCount;
}

void MMMutableArray_removeLastObject(MMMutableArray * recv){
    MMMutableArray_removeObjectAtIndex(recv, recv->count-1);
}

MMMutableArray * MMMutableArray_copy(const MMMutableArray * recv){
    return (MMMutableArray *) MMArray_copy(recv);
}

void MMMutableArray_release(MMMutableArray *recv) {
    MMArray_release(recv);
}


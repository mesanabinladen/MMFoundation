#include "MMDictionary.h"
#include "MMTypes.h"
#include "MMMemoryManagement.h"
#include "MMString.h"
#include <stdarg.h>
#include <stdio.h>

MMDictionary * MMDictionary_init(){
    MMDictionary * n = MM_init(MMTypeDictionary);
    n->entries = nil;
    n->count = 0;
    n->capacity = 0;
    return n;
}

MMDictionary * MMDictionary_dictionaryWithObjectsAndKeys(void *firstObject, ...){
    MMDictionary *dictionary = MMDictionary_init();
    if (!dictionary || !firstObject) return dictionary;

    va_list args;
    va_start(args, firstObject);
    void *object = firstObject;

    while (object) {
        MMString *key = va_arg(args, MMString *);
        if (!key) {
            va_end(args);
            MM_release(dictionary);
            return nil;
        }

        MMMutableDictionary_setObjectForKey((MMMutableDictionary *)dictionary, object, key);
        object = va_arg(args, void *);
    }

    va_end(args);
    return dictionary;
}

void * MMDictionary_objectForKey(const MMDictionary *recv, const MMString *key){
    if (!recv || !key) return nil;

    for (size_t i = 0; i < recv->count; i++) {
        if (MMString_isEqualToString(recv->entries[i].key, (MMString *)key)) {
            return recv->entries[i].object;
        }
    }
    return nil;
}

void * MMDictionary_objectForCStringKey(const MMDictionary *recv, const char *key){
    if (!recv || !key) return nil;

    for (size_t i = 0; i < recv->count; i++) {
        if (MMString_isEqualToCString(recv->entries[i].key, key)) {
            return recv->entries[i].object;
        }
    }
    return nil;
}

MMDictionary * MMDictionary_copy(const MMDictionary * recv){
    if (!recv) return nil;
    ObjectType * ptr = (ObjectType *)recv;
    MMDictionary *newPtr = malloc(sizeof(MMDictionary));
    if (!newPtr) return nil;
    memcpy(newPtr, ptr, sizeof(MMDictionary));
    newPtr->retainCount = 1;
    newPtr->capacity = recv->count;
    newPtr->entries = recv->count ? malloc(recv->count * sizeof(MMDictionaryEntry)) : nil;
    if (recv->count && !newPtr->entries) {
        free(newPtr);
        return nil;
    }

    for (size_t i = 0; i < recv->count; i++) {
        newPtr->entries[i] = recv->entries[i];
        MM_retain(newPtr->entries[i].key);
        MM_retain(newPtr->entries[i].object);
    }

    return newPtr;
}

void MMDictionary_release(MMDictionary * recv){
    if (!recv) return;
    for (size_t i = 0; i < recv->count; i++) {
        MM_release(recv->entries[i].key);
        MM_release(recv->entries[i].object);
    }
    free(recv->entries);
    recv->entries = nil;
    recv->count = 0;
    recv->capacity = 0;
    free(recv);
}

MMMutableDictionary * MMMutableDictionary_init(){
    return (MMMutableDictionary *) MMDictionary_init();
}

void MMMutableDictionary_setObjectForKey(MMMutableDictionary *recv, void *object, MMString *key){
    if (!recv || !object || !key) return;

    for (size_t i = 0; i < recv->count; i++) {
        if (MMString_isEqualToString(recv->entries[i].key, key)) {
            MM_retain(object);
            MM_release(recv->entries[i].object);
            recv->entries[i].object = object;
        }
    }

    if (recv->count == recv->capacity) {
        size_t newCapacity = recv->capacity == 0 ? 4 : recv->capacity * 2;
        MMDictionaryEntry *newEntries = realloc(recv->entries, newCapacity * sizeof(MMDictionaryEntry));
        if (!newEntries) {
            printf("Error adding object to dictionary\n");
            exit(1);
        }
        recv->entries = newEntries;
        recv->capacity = newCapacity;
    }

    MM_retain(key);
    MM_retain(object);
    recv->entries[recv->count].key = key;
    recv->entries[recv->count].object = object;
    recv->count++;
}

MMMutableDictionary * MMMutableDictionary_copy(const MMMutableDictionary * recv){
    return (MMMutableDictionary *) MMDictionary_copy((MMDictionary *)recv);
}
void MMMutableDictionary_release(MMMutableDictionary *recv){
    MMDictionary_release((MMDictionary *)recv);
}
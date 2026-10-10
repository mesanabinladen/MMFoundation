
#ifndef MMDICTIONARY_H
#define MMDICTIONARY_H

#include "MMTypes.h"

typedef struct {
    MMString *key;
    void *object;
} MMDictionaryEntry;

typedef struct {
    int type;
    int retainCount;
    //-------------
    MMDictionaryEntry *entries;
    size_t count;
    size_t capacity;
} MMDictionary;

MMDictionary * MMDictionary_init();
MMDictionary * MMDictionary_dictionaryWithObjectsAndKeys(void *firstObject, ...);
void * MMDictionary_objectForKey(const MMDictionary *recv, const MMString *key);
void * MMDictionary_objectForCStringKey(const MMDictionary *recv, const char *key);

//MUTABLE DICTIONARY
typedef MMDictionary MMMutableDictionary;

MMMutableDictionary * MMMutableDictionary_init();
void MMMutableDictionary_setObjectForKey(MMMutableDictionary *recv, void *object, MMString *key);

#endif
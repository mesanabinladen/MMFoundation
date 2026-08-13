#ifndef MMDATA_H
#define MMDATA_H

#include <stddef.h>
#include "MMString.h"
#include "MMTypes.h"

typedef struct MMData {
    int type;
    int retainCount;
    //-------------
    void *buffer;
    size_t length;
} MMData;

typedef enum {
    None = 0,
    MMDataSearchBackwards //Search from the end of the data object.
    //NSDataSearchAnchored,
} MMDataSearchOptions;

MMData *MMData_initWithCapacity(size_t size);
MMData *MMData_initWithBytes(const void *bytes, size_t length);
MMData *MMData_initWithContentsOfFile(const MMString *path);
MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc);
MMRange MMData_rangeOfData(const MMData *recv, MMData *dataToFind, MMDataSearchOptions mask, MMRange searchRange);
void MMData_release(MMData *recv);

typedef MMData MMMutableData ;

MMMutableData *MMMutableData_initWithCapacity(size_t size);
MMMutableData *MMMutableData_initWithBytes(const void *bytes, size_t length);
MMMutableData *MMMutableData_initWithContentsOfFile(MMString *path);
void MMMutableData_appendBytes(MMMutableData * recv, const void * bytes, MMUInteger length);
void MMMutableData_appendData(MMMutableData * recv, MMData * other);
void MMMutableData_release(MMMutableData *recv);

#endif /*MMDATA_H*/

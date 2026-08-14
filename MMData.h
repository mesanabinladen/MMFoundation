#ifndef MMDATA_H
#define MMDATA_H

#include "MMTypes.h"

typedef struct MMData {
    int type;
    int retainCount;
    //-------------
    void *bytes;
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
void MMData_getBytes(const MMData* recv, void * buffer , MMUInteger length);
MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc);
MMRange MMData_rangeOfData(const MMData *recv, MMData *dataToFind, MMDataSearchOptions mask, MMRange searchRange);
MMBool MMData_writeToFile(const MMData *recv, const MMString * path, MMBool useAuxiliaryFile);
MMData *MMData_copy(MMData * recv);

typedef struct MMMutableData {
    int type;
    int retainCount;
    //-------------
    union{
        void *bytes;
        void *mutableBytes;
    };
    size_t length;
} MMMutableData;

MMMutableData *MMMutableData_initWithCapacity(size_t size);
MMMutableData *MMMutableData_initWithBytes(const void *bytes, size_t length);
MMMutableData *MMMutableData_initWithContentsOfFile(MMString *path);
void MMutableData_getBytes(const MMMutableData * recv, void * buffer , MMUInteger length);
void MMMutableData_appendBytes(MMMutableData * recv, const void * bytes, MMUInteger length);
void MMMutableData_appendData(MMMutableData * recv, MMData * other);
MMBool MMMutableData_writeToFile(const MMMutableData *recv, const MMString *path, MMBool useAuxiliaryFile);
MMMutableData *MMMutableData_copy(MMMutableData * recv);

#endif /*MMDATA_H*/

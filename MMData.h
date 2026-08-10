#ifndef MMDATA_H
#define MMDATA_H

#include <stddef.h>
#include "MMString.h"
#include "MMTypes.h"

typedef struct MMData {
    void *buffer;
    size_t length;
} MMData;

typedef enum {
    /*
    MMUTF16BigEndianStringEncoding,
    MMUTF16StringEncoding,
    MMUTF16LittleEndianStringEncoding,
    MMUTF16StringEncoding,
    MMUTF16StringEncoding,
    MMUnicodeStringEncoding,
    MMUTF32BigEndianStringEncoding,
    MMUTF32StringEncoding,
    MMUTF32LittleEndianStringEncoding,
    MMUTF32StringEncoding,
    MMUTF32StringEncoding,
    */
    MMUTF8StringEncoding
} MMStringEncoding;

typedef enum {
    None = 0,
    MMDataSearchBackwards //Search from the end of the data object.
    //NSDataSearchAnchored,
} MMDataSearchOptions;

MMData *MMData_initWithCapacity(size_t size);
MMData *MMData_initWithBytes(const void *bytes, size_t length);
MMData *MMData_initWithContentsOfFile(MMString *path);

MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc);

MMRange MMData_rangeOfData(MMData *self, MMData *dataToFind, MMDataSearchOptions mask, MMRange searchRange);

void MMData_release(MMData *data);

#endif /* MMDATA_H */

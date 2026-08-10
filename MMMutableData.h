#ifndef MMMUTABLEDATA_H
#define MMMUTABLEDATA_H

#include <stddef.h>
#include "MMData.h"

typedef MMData MMMutableData ;

MMMutableData *MMMutableData_initWithCapacity(size_t size);
MMMutableData *MMMutableData_initWithBytes(const void *bytes, size_t length);
MMMutableData *MMMutableData_initWithContentsOfFile(MMString *path);

void MMMutableData_release(MMMutableData *data);

#endif /* MMMUTABLEDATA_H */

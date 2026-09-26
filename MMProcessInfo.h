#ifndef MMPROCESSINFO_H
#define MMPROCESSINFO_H

#include "MMTypes.h"

typedef struct MMProcessInfo {
    int type;
    int retainCount;
    //-------------
    unsigned int processorCount;
} MMProcessInfo;

MMProcessInfo *MMProcessInfo_init(void);
MMProcessInfo *MMProcessInfo_copy(const MMProcessInfo *recv);

#endif /* MMPROCESSINFO_H */
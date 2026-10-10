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

#endif /* MMPROCESSINFO_H */
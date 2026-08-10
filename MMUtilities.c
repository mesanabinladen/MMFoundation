#include "MMUtilities.h"
#include <stdlib.h>

MMRange MMMakeRange(MMUInteger loc, MMUInteger len){
    MMRange r;
    r.location = loc;
    r.length   = len;
    return r;   
}


#include "MMRange.h"

MMRange MMMakeRange(MMUInteger loc, MMUInteger len){
    MMRange r;
    r.location = loc;
    r.length   = len;
    return r;   
}
#include "MMUtilities.h"

#include <stdlib.h>
#include <stdio.h>

#include "MMArray.h"
#include "MMData.h"
#include "MMDate.h"
#include "MMFileHandle.h"
#include "MMString.h"
#include "MMNumber.h"

MMRange MMMakeRange(MMUInteger loc, MMUInteger len){
    MMRange r;
    r.location = loc;
    r.length   = len;
    return r;   
}

//memory management
void MM_release(void* anObject){
    ObjectType * ptr = (ObjectType *)anObject;
    ptr->retainCount--;
    if (ptr->retainCount<0){
        printf("ERROR - RETAIN COUNT LESS THAN 0\n");
        exit(1);
    }   
    else if (ptr->retainCount==0)//remove object from memory only if retainCount is zero
    {
        switch (ptr->type) {
            case (MMTypeArray):
                MMArray_release((MMArray *)ptr);
                break;
            case (MMTypeData):
                MMData_release((MMData *)ptr);
                break;
            case (MMTypeDate):
                MMDate_release((MMDate *)ptr);
                break;
            case (MMTypeFileHandle):
                MMFileHandle_closeFile((MMFileHandle *)ptr);
                break;
            case (MMTypeString):
                MMString_release((MMString *)ptr);
                break;
            case (MMTypeNumber):
                MMNumber_release((MMNumber *)ptr);     
            default:
                printf("Error releasing object type %i!\n", ptr->type);
                exit(1);
        }
    }
}

#include "MMMemoryManagement.h"

#include "MMArray.h"
#include "MMData.h"
#include "MMDate.h"
#include "MMFileHandle.h"
#include "MMString.h"
#include "MMNumber.h"


//autoreleasepool
static int poolLevel = 0;
static MMMutableArray * poolObjects = nil;

void MMAutoreleasePool_init(){
    if (poolObjects)
    {
        MMAutoreleasePool_drain();
    }
    poolObjects = MMMutableArray_init();
    //pool must be enabled AFTER the initialization!
    poolLevel++;
}

void MMAutoreleasePool_drain(){

    MMMutableArray_release(poolObjects);
    poolObjects = nil;
    poolLevel--;
    poolObjects = poolLevel>0 ? MMMutableArray_init() : nil;    
}

//memory management
void * MM_init(int type){
    void * voidPtr;
    switch (type) {
        case (MMTypeArray):
            voidPtr = malloc(sizeof(MMArray));
            break;
        case (MMTypeMutableArray):
            voidPtr = malloc(sizeof(MMMutableArray));
            break;
        case (MMTypeData):
            voidPtr = malloc(sizeof(MMData));
            break;
        case (MMTypeMutableData):
            voidPtr = malloc(sizeof(MMMutableData));
            break;
        case (MMTypeDate):
            voidPtr = malloc(sizeof(MMDate));
            break;
        case (MMTypeFileHandle):
            voidPtr = malloc(sizeof(MMFileHandle));
            break;
        case (MMTypeString):
            voidPtr = malloc(sizeof(MMString));
            break;
        case (MMTypeMutableString):
            voidPtr = malloc(sizeof(MMMutableString));
            break;
        case (MMTypeNumber):
            voidPtr = malloc(sizeof(MMNumber));
            break;
        default:
            printf("Error initializig object type %i!\n", type);
            exit(1);
    }
    if (!voidPtr){
        printf("Error initializing object of type %i\n",type);
        exit(1);
    }
    ObjectType * ptr = (ObjectType *)voidPtr;
    ptr->type = type;
    ptr->retainCount = 1;
    if (poolLevel>0){
        MMMutableArray_addObject(poolObjects, ptr);
    }
    return ptr;
}

void * MM_retain(void* anObject){
    ObjectType * ptr = (ObjectType *)anObject;
    ptr->retainCount++;
    return ptr;
}

void * MM_copy(void * anObject){
    if (!anObject) return nil;

    ObjectType * ptr = (ObjectType *)anObject;
    void * newPtr = nil;

    size_t objSize = 0;
    switch (ptr->type) {
         case (MMTypeArray):
            newPtr = MMArray_copy((MMArray *) ptr);
            break;
        case (MMTypeMutableArray):
            newPtr = MMMutableArray_copy((MMMutableArray *) ptr);
            break;
        case (MMTypeData):
            newPtr = MMData_copy((MMData *) ptr);
            break;
        case (MMTypeMutableData):
            newPtr = MMMutableData_copy((MMMutableData *) ptr);
            break;
        case (MMTypeDate):
            objSize = sizeof(MMDate);
            //no subclassing of MM_copy!
            break;
        case (MMTypeFileHandle):
            objSize = sizeof(MMFileHandle);
            //no subclassing of MM_copy!
            break;
        case (MMTypeString):
            newPtr = MMString_copy((MMString *) ptr);
            break;
        case (MMTypeMutableString):
            newPtr = MMMutableString_copy((MMMutableString *) ptr);
            break;
        case (MMTypeNumber):
            objSize = sizeof(MMNumber);
            //no subclassing of MM_copy!
            break;
        default:
            printf("Error initializig object type %i!\n", ptr->type);
            exit(1);
    }
    if (newPtr==nil){ //for simple objects with no subclassing of MM_copy...
        newPtr = malloc(sizeof(objSize));
        memcpy(newPtr, ptr, objSize);
        ((ObjectType *)newPtr)->retainCount = 1;
    }
    return newPtr;
}


void MM_release(void* anObject){
    if (anObject==nil) return;
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
            case (MMTypeMutableArray):
                MMMutableArray_release((MMMutableArray *)ptr);
                break;
            case (MMTypeData):
                MMData_release((MMData *)ptr);
                break;
            case (MMTypeMutableData):
                MMMutableData_release((MMMutableData *)ptr);
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
            case (MMTypeMutableString):
                MMMutableString_release((MMMutableString *)ptr);
                break;
            case (MMTypeNumber):
                MMNumber_release((MMNumber *)ptr);     
            default:
                printf("Error releasing object type %i!\n", ptr->type);
                exit(1);
        }
    }
}


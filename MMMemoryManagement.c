#include "MMMemoryManagement.h"

#include "MMFoundation.h"

#include "MMReleases.h"

//autoreleasepool
MMMutableArray * pools = nil;
MMMutableArray *actualPool = nil;

void MMAutoreleasePool_init(){  
    if (!pools)
    {
        pools=MMMutableArray_initWithCapacity(0);
    }
    actualPool = MMMutableArray_init();
    MMMutableArray_addObject(pools, actualPool);
}

void MMAutoreleasePool_drain(){
    if (pools->count>0) {
        MMMutableArray_removeLastObject(pools);  
        actualPool = pools->count > 0 ? pools->items[pools->count-1] : nil;
    }
    else{
        printf("Error! Attempt to drain an empty pool\n");
        exit(1);
    }
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
        case (MMTypeLock):
            voidPtr = malloc(sizeof(MMLock));
            break;
        case (MMTypeURLRequest):
            voidPtr = malloc(sizeof(MMURLRequest));
            break;
        case (MMTypeURL):
            voidPtr = malloc(sizeof(MMURL));
            break;
        case (MMTypeHTTPURLResponse):
            voidPtr = malloc(sizeof(MMHTTPURLResponse));
            break;
        case (MMTypeError):
            voidPtr = malloc(sizeof(MMError));
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
    if (actualPool){
        MMMutableArray_addObject(actualPool, ptr);
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
        case (MMTypeLock):
            objSize = sizeof(MMLock);
            //no subclassing of MM_copy!
            break;
        case (MMTypeURLRequest):
            newPtr = MMURLRequest_copy((MMURLRequest *) ptr);
            break;
        case (MMTypeURL):
            newPtr = MMURL_copy((MMURL *) ptr);
            break;
        case (MMTypeHTTPURLResponse):
            newPtr = MMTTPURLResponse_copy((MMHTTPURLResponse *) ptr);
            break;
        case (MMTypeError):
            newPtr = MMError_copy((MMError *) ptr);
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
                MMFileHandle_release((MMFileHandle *)ptr);
                break;
            case (MMTypeString):
                MMString_release((MMString *)ptr);
                break;
            case (MMTypeMutableString):
                MMMutableString_release((MMMutableString *)ptr);
                break;
            case (MMTypeNumber):
                MMNumber_release((MMNumber *)ptr);  
                break; 
            case (MMTypeLock):
                break;
                MMLock_release((MMLock *)ptr);    
            case (MMTypeURLRequest):
                MMURLRequest_release((MMURLRequest *)ptr);  
                break;
            case (MMTypeURL):
                MMURL_release((MMURL *)ptr);  
                break;
            case (MMTypeHTTPURLResponse):
                MMHTTPURLResponse_release((MMHTTPURLResponse *)ptr);
                break;
            case (MMTypeError):
                MMError_release((MMError *) ptr);
                break;
            default:
                printf("Error releasing object type %i!\n", ptr->type);
                exit(1);
        }
    }
}


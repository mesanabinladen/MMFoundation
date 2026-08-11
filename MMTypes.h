#ifndef MMTYPES_H
#define MMTYPES_H

#include <stddef.h>

#define YES 1
#define NO 0
#define nil 0

#define MMNotFound  ((size_t)-1) 

typedef unsigned char BOOL;
typedef unsigned long MMUInteger;
typedef long MMInteger;

typedef enum {
    MMTypeNone,
    MMTypeFreed,
    MMTypeArray,
    MMTypeData,
    MMTypeDate,
    MMTypeFileHandle,
    MMTypeString,
    MMTypeNumber
} ObjectTypes;

typedef struct  {
    int type;
    int retainCount;
} ObjectType;

typedef struct MMString {
    int type;
    int retainCount;
    //-------------
    char *cstring;
    size_t length;
} MMString;

typedef MMString * MMErrorDomain;

typedef struct MMError {
    int type;
    int retainCount;
    //-------------
   MMUInteger code;// The error code.
   MMErrorDomain * domain; //A string containing the error domain.
   //NSDictionary<NSString *,id>  userInfo;// The user info dictionary NOT IMPLEMENTED YET.
} MMError;

typedef struct MMRange {
    MMUInteger location;
    MMUInteger length;
} MMRange;

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


#endif /*MMTYPES_H*/
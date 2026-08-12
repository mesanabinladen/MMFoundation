#ifndef MMTYPES_H
#define MMTYPES_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "MMMemoryManagement.h"

#if defined(_WIN32) || defined(_WIN64)
    #define MM_PATH_SEPARATOR (char)'\\'
    #define MMNotFound  (UINT_MAX-1) //32 bit on windows
    #include <io.h>
#else
    #define MM_PATH_SEPARATOR (char)'/'
    #define MMNotFound  (UINT32_MAX-1) //32 bit on posix
    #include <unistd.h>
#endif 

#define YES 1
#define NO 0
#define nil ((void*)0)

/* Use a project-local boolean type to avoid conflicting with Windows' BOOL (which is an int).
    Prefer migrating code to use `MMBool` to avoid ABI conflicts with WinAPI. */

typedef unsigned char MMBool;

typedef unsigned long MMUInteger;
typedef long MMInteger;

typedef enum {
    MMTypeNone,
    MMTypeFreed,
    MMTypeArray,
    MMTypeMutableArray,
    MMTypeData,
    MMTypeMutableData,
    MMTypeDate,
    MMTypeFileHandle,
    MMTypeString,
    MMTypeMutableString,
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
    MMASCIIStringEncoding,
    MMUTF8StringEncoding
} MMStringEncoding;


#endif /*MMTYPES_H*/
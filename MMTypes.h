#ifndef MMTYPES_H
#define MMTYPES_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
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
    MMTypeDictionary,
    MMTypeMutableDictionary,
    MMTypeData,
    MMTypeMutableData,
    MMTypeDate,
    MMTypeFileHandle,
    MMTypeString,
    MMTypeMutableString,
    MMTypeNumber,
    MMTypeLock,
    MMTypeURLRequest,
    MMTypeURL,
    MMTypeHTTPURLResponse,
    MMTypeError,
    MMTypeOperationQueue,
    MMTypeInvocationOperation,
    MMTypeProcessInfo,
    MMTypeValue
} ObjectTypes;

typedef struct  {
    int type;
    int retainCount;
} ObjectType;

typedef struct MMString {
    int type;
    int retainCount;
    //-------------
    char *cString;
    size_t length;
} MMString;

typedef MMString * MMErrorDomain;

typedef struct MMError {
    int type;
    int retainCount;
    //-------------
   MMUInteger code;// The error code.
   MMErrorDomain domain; //A string containing the error domain.
   MMString *localizedDescription;
} MMError;

typedef struct MMRange {
    MMUInteger location;
    MMUInteger length;
} MMRange;

typedef MMRange * MMRangePointer;

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

typedef enum {
    MMOrderedAscending  = -1, //The left operand is smaller than the right operand.
    MMOrderedSame, //The two operands are equal.
    MMOrderedDescending
} MMComparisonResult;

typedef MMInteger MMStringCompareOptions;

enum {
    MMCaseInsensitiveSearch      = 1 << 0, //A case-insensitive search.
    MMLiteralSearch              = 1 << 1, //Exact character-by-character equivalence.
    MMBackwardsSearch            = 1 << 2, //Search from end of source string.
    MMAnchoredSearch             = 1 << 3, //Search is limited to start (or end, if NSBackwardsSearch) of source string.
    MMNumericSearch              = 1 << 4, //Numbers within strings are compared using numeric value, that is, Name2.txt < Name7.txt < Name25.txt.
    MMDiacriticInsensitiveSearch = 1 << 5, //Search ignores diacritic marks.
    MMWidthInsensitiveSearch     = 1 << 6, //Search ignores width differences in characters that have full-width and half-width forms, as occurs in East Asian character sets.
    MMForcedOrderingSearch       = 1 << 7, //Comparisons are forced to return either NSOrderedAscending or NSOrderedDescending if the strings are equivalent but not strictly equal.
    MMRegularExpressionSearch    = 1 << 8 //The search string is treated as an ICU-compatible regular expression. If set, no other options can apply except NSCaseInsensitiveSearch and NSAnchoredSearch. You can use this option only with the rangeOfString:… methods and stringByReplacingOccurrencesOfString:withString:options:range:.
};

typedef enum {
    MMDataReadingMappedIfSafe = 1 << 0, //Map the file when safe; otherwise read it into memory.
    MMDataReadingUncached = 1 << 1, //Avoid caching the file contents when the platform supports it.
    MMDataReadingMappedAlways = 1 << 3 //Require the file to be memory-mapped.
} MMDataReadingOptions;

#endif /*MMTYPES_H*/
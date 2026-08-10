#ifndef MMTYPES_H
#define MMTYPES_H

#define YES 1
#define NO 0
#define nil 0

#define MMNotFound  ((size_t)-1) 

typedef unsigned char BOOL;
typedef unsigned long MMUInteger;

typedef struct MMRange {
    MMUInteger location;
    MMUInteger length;
} MMRange;

#endif /*MMTYPES_H*/
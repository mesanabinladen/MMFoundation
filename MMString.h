#ifndef MMSTRING_H
#define MMSTRING_H

#include <stddef.h>
#include "MMTypes.h"
#include "MMArray.h"

MMString *MMString_initWithCString(const char *cString);
MMString *MMString_initWithUTF8String(const char *nullTerminatedCString);
MMString *MMString_initWithFormat(const char *format, ...);
const char *MMString_cStringUsingEncoding(const MMString *recv, MMStringEncoding enc);
MMString *MMString_stringByDeletingLastPathComponent(const MMString *recv);
MMString *MMString_stringByAppendingString(const MMString *recv, const MMString *append);
MMString *MMString_stringByAppendingPathComponent(const MMString *recv, MMString *str);
MMString *MMString_stringByAppendingCPathComponent(const MMString *recv, char *str);
MMBool MMString_writeToFile(const MMString *recv, const MMString *path, MMBool useAuxiliaryFile);
MMString *MMString_stringWithContentsOfFile(MMString * path, MMStringEncoding enc, MMError *error);
MMArray *MMString_componentsSeparatedByString(const MMString *recv, MMString * separator);
MMInteger MMString_integerValue(const MMString *recv);
MMRange MMString_rangeOfString(const MMString *recv, MMString *searchString);
MMRange MMString_rangeOfCString(const MMString *recv, const char *str);
MMUInteger MMString_hash(const MMString *recv);
MMBool MMString_isEqualToString(const MMString *recv, MMString * aString);
MMBool MMString_isEqualToCString(const MMString *recv, const char *str);
MMUInteger MSString_lengthOfBytesUsingEncoding(const MMString *recv, MMStringEncoding enc);
MMString *MMString_stringByReplacingOccurrencesOfString(const MMString *recv, MMString *target, MMString *replacement);
MMBool MMString_getCString(const MMString *recv, char *buffer, MMUInteger maxBufferCount, MMStringEncoding encoding);
const char *MMString_fileSystemRepresentation(const MMString *recv);
MMBool MMString_hasSuffix(const MMString *recv, MMString *str);
MMString *MMString_substringFromIndex(const MMString *recv, MMUInteger from);
MMString *MMString_substringToIndex(const MMString *recv, MMUInteger to);
long MMString_longValue(const MMString *recv);
long long MMString_longLongValue(const MMString *recv);
double MMString_doubleValue(const MMString *recv);
MMString *MMString_copy(MMString *recv);
void MMString_release(MMString *recv);

typedef MMString MMMutableString ;

MMMutableString *MMMutableString_initWithCString(const char *str);
void MMMutableString_appendString(MMMutableString *recv, MMString * aString);
MMMutableString *MMMutableString_copy(MMMutableString * recv);
void MMMutableString_release(MMMutableString *recv);

#endif /*MMSTRING_H*/
#ifndef MMSTRING_H
#define MMSTRING_H

#include <stddef.h>
#include "MMTypes.h"
#include "MMArray.h"

/* Create an MMString with a C string */
MMString *MMString_initWithCString(const char *str);

MMString *MMString_initWithUTF8String(const char *str);

/* Create an MMString using a printf-style format string */
MMString *MMString_initWithFormat(const char *format, ...);

/* Return the C string */
const char *MMString_cString(const MMString *str);

const char *MMString_cStringUsingEncoding(const MMString *str, MMStringEncoding enc);

/* Return a new MMString with the last path component removed  */
MMString *MMString_stringByDeletingLastPathComponent(const MMString *path);

/* Return a new MMString with a string appended */
MMString *MMString_stringByAppendingString(const MMString *base, const MMString *append);

MMString * MMString_stringByAppendingPathComponent(MMString * base, MMString *str);

/* Write the contents of an MMString to a file */
MMBool MMString_writeToFile(const MMString *str, const MMString *path, MMBool atomically);

MMString * MMString_stringWithContentsOfFile(MMString * path, MMStringEncoding enc, MMError *error);

MMArray * MMString_componentsSeparatedByString(MMString * str, MMString * separator);

MMInteger MMString_integerValue(MMString *str);

MMRange MMString_rangeOfString(MMString *str, MMString *searchString);

MMUInteger MMString_hash(MMString *str);

MMBool MMString_isEqualToString(MMString *str, MMString * aString);

MMUInteger MSString_lengthOfBytesUsingEncoding(MMString *str, MMStringEncoding enc);

MMString * MMString_stringByReplacingOccurrencesOfString(MMString *str, MMString *target, MMString *replacement);

/* Free the MMString */
void MMString_release(MMString *str);

typedef MMString MMMutableString ;


#endif /* MMSTRING_H */
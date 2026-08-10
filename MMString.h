#ifndef MMSTRING_H
#define MMSTRING_H

#include <stddef.h>
#include "MMTypes.h"

typedef struct MMString {
    char *cstring;
    size_t length;
} MMString;

/* Create an MMString with a C string */
MMString *MMString_initWithCString(const char *str);

/* Create an MMString using a printf-style format string */
MMString *MMString_initWithFormat(const char *format, ...);

/* Return the C string */
const char *MMString_cString(const MMString *str);

/* Build a path from a directory and a filename */
MMString *MMString_build_path(const MMString *dir, const MMString *filename);

/* Return a new MMString with the last path component removed  */
MMString *MMString_stringByDeletingLastPathComponent(const MMString *path);

/* Return a new MMString with a string appended */
MMString *MMString_stringByAppendingString(const MMString *base, const MMString *append);

/* Write the contents of an MMString to a file */
BOOL MMString_writeToFile(const MMString *str, const MMString *path, BOOL atomically);

/* Free the MMString */
void MMString_release(MMString *str);

#endif /* MMSTRING_H */
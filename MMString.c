#include "MMString.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "MMTypes.h"
#include <stdbool.h>

/*Create an empty MMString */
MMString * MMString_init(size_t capacity){
    MMString *str = (MMString*)malloc(sizeof(MMString));
    if (!str) return NULL;
    if (capacity <= 0) {
        str->cstring = nil;
    } 
    else{
        str->cstring = realloc(str->cstring, capacity + 1);
        str->cstring[capacity] = '\0';
    }   
    str->length = capacity;
    return str;
}

MMString *MMString_initWithCString(const char *str) {

    size_t len = strlen(str);
    MMString *newStr = MMString_init(len);
    if (!newStr) return NULL;
    strcpy(newStr->cstring, str);
    return newStr;
}

MMString *MMString_initWithFormat(const char *format, ...) {
    if (!format) {
        return NULL;
    }

    va_list args;
    va_start(args, format);
    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (needed < 0) {
        return NULL;
    }

    MMString *newStr = MMString_init((size_t)needed);
    if (!newStr) {
        return NULL;
    }

    va_start(args, format);
    vsnprintf(newStr->cstring, (size_t)needed + 1, format, args);
    va_end(args);

    newStr->length = (size_t)needed;
    return newStr;
}

//C function to build a path from a directory and a filename
MMString *MMString_build_path(const MMString *dir, const MMString *filename) {
    if (!dir || !filename) return NULL;

    int need_sep = (dir->length > 0 && dir->cstring[dir->length - 1] != '/');

    size_t total = dir->length + need_sep + filename->length + 1;
    char *path = malloc(total);
    if (!path) return NULL;

    if (need_sep)
        snprintf(path, total, "%s/%s", dir->cstring, filename->cstring);
    else
        snprintf(path, total, "%s%s", dir->cstring, filename->cstring);

    return MMString_initWithCString(path);
}

MMString *MMString_stringByDeletingLastPathComponent(const MMString *path){
    if (!path || !path->cstring) return NULL;

    char *lastSlash = strrchr(path->cstring, '/');
    if (!lastSlash) {
        // No slash found, return an empty string
        MMString *newStr = MMString_init(0);
        return newStr;
    }

    size_t newLength = lastSlash - path->cstring;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return NULL;

    strncpy(newStr->cstring, path->cstring, newLength);

    return newStr;
}

MMString *MMString_stringByAppendingString(const MMString *base, const MMString *append){
    if (!base || !append) return NULL;

    size_t newLength = base->length + append->length;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return NULL;

    strcpy(newStr->cstring, base->cstring);
    strcat(newStr->cstring, append->cstring);
    return newStr;

}

BOOL MMString_writeToFile(const MMString *str, const MMString *path, BOOL atomically) {
    if (!str || !path || !str->cstring || !path->cstring) {
        return NO;
    }

    FILE *file = fopen(path->cstring, atomically ? "w" : "w");
    if (!file) {
        return NO;
    }

    size_t written = fwrite(str->cstring, 1, str->length, file);
    fclose(file);

    return written == str->length;
}

const char *MMString_cString(const MMString *str) {
    return str ? str->cstring : NULL;
}       

//release
void MMString_release(MMString *str) {
    if (!str) return;
    free(str->cstring);
    free(str);
}



 
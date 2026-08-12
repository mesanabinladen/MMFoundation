#include "MMTypes.h"

#include "MMString.h"
#include "MMData.h"
#include "MMError.h"
#include "MMArray.h"

/*Create an empty MMString */
MMString * MMString_init(size_t capacity){
    MMString *str = MM_init(MMTypeString);

    if (capacity < 0) {
        str->cstring = nil;
    } 
    else{ //if capacity is zero, it means a nil terminated string!
        str->cstring = malloc(capacity + 1);
        if (!str->cstring) {
            free(str);
            return nil;
        }
        str->cstring[capacity] = '\0';
    }  
    str->length = capacity;

    return str;
}

MMString *MMString_initWithCString(const char *str) {

    size_t len = strlen(str);
    MMString *newStr = MMString_init(len);
    if (!newStr) return nil;
    strcpy(newStr->cstring, str);
    return newStr;
}

MMString *MMString_initWithUTF8String(const char *str) {
    return MMString_initWithCString(str);
}

MMString *MMString_initWithFormat(const char *format, ...) {
    if (!format) {
        return nil;
    }

    va_list args;
    va_start(args, format);
    int needed = vsnprintf(nil, 0, format, args);
    va_end(args);

    if (needed < 0) {
        return nil;
    }

    MMString *newStr = MMString_init((size_t)needed);
    if (!newStr) {
        return nil;
    }

    va_start(args, format);
    vsnprintf(newStr->cstring, (size_t)needed + 1, format, args);
    va_end(args);

    newStr->length = (size_t)needed;
    return newStr;
}

MMString *MMString_stringByDeletingLastPathComponent(const MMString *path){
    if (!path || !path->cstring) return nil;

    char *lastSlash = strrchr(path->cstring, MM_PATH_SEPARATOR);

    if (!lastSlash) {
        // No slash found, return an empty string
        MMString *newStr = MMString_init(0);
        return newStr;
    }

    size_t newLength = lastSlash - path->cstring;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strncpy(newStr->cstring, path->cstring, newLength);

    return newStr;
}

MMString *MMString_stringByAppendingString(const MMString *base, const MMString *append){
    if (!base || !append) return nil;

    size_t newLength = base->length + append->length;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strcpy(newStr->cstring, base->cstring);
    strcat(newStr->cstring, append->cstring);
    return newStr;

}

MMString * MMString_stringByAppendingPathComponent(MMString * base, MMString *str){
     if (!base || !str) return nil;

    size_t newLength = base->length + str->length + 1; //+ 1 for separator
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strcpy(newStr->cstring, base->cstring);

    char separator[2] = {MM_PATH_SEPARATOR, '\0'};
    strcat(newStr->cstring, separator);

    strcat(newStr->cstring, str->cstring);
    return newStr;
}


MMBool MMString_writeToFile(const MMString *str, const MMString *path, MMBool atomically) {
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
    return str ? str->cstring : nil;
}       

const char *MMString_cStringUsingEncoding(const MMString *str, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return MMString_cString(str);
}


MMString * MMString_stringWithContentsOfFile(MMString * path, MMStringEncoding enc, MMError *error){
    if (error){
        printf("Error codes not implemented yet!");
        exit(1);
    }
    if (!path || !path->cstring) return nil;

    MMData *data = MMData_initWithContentsOfFile(path);
    if (!data) return nil;

    //MMStringEncoding are not yet implemented!
    MMString *str = MMString_init(data->length);
    if (!str) {
        MMData_release(data);
        return nil;
    }

    if (str->length > 0) {
        memcpy(str->cstring, data->buffer, str->length);
    }

    MMData_release(data);
    return str;
}

MMArray * MMString_componentsSeparatedByString(MMString * str, MMString * separator){
    if (!str || !separator || !str->cstring || !separator->cstring) return nil;
    if (separator->length == 0) return nil;

    MMMutableArray *result = MMMutableArray_init(); //
    if (!result) return nil;

    size_t start = 0;
    size_t sepLen = separator->length;
    while (start <= str->length) {
        size_t found = MMNotFound;
        for (size_t i = start; i + sepLen <= str->length; i++) {
            if (memcmp(str->cstring + i, separator->cstring, sepLen) == 0) {
                found = i;
                break;
            }
        }

        size_t partLen;
        if (found == MMNotFound) {
            partLen = str->length - start;
        } else {
            partLen = found - start;
        }

        MMString *part = MMString_init(partLen);
        if (!part) {
            for (size_t idx = 0; idx < result->count; idx++) {
                MMString_release((MMString *)result->items[idx]);
            }
            MMMutableArray_release(result);
            return nil;
        }
        if (partLen > 0) {
            memcpy(part->cstring, str->cstring + start, partLen);
        }
        part->retainCount = 0; //we don't need to retain 2 times the temporary object
        MMMutableArray_addObject(result, part);

        if (found == MMNotFound) {
            break;
        }
        start = found + sepLen;
    }

    return (MMArray *)result;
}

//This property is 0 if the string doesn’t begin with a valid decimal text representation of a number.
MMInteger MMString_integerValue(MMString *str) {
    char *end;
    if (str->cstring[0] == '\0' || str->cstring[0] == ' ')
        return 0;
    errno = 0;
    long l = strtol(str->cstring, &end, 10);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return 0;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return 0;
    if (*end != '\0')
        return 0;
    return l;
}

MMRange MMString_rangeOfString(MMString *str, MMString *searchString) {

    MMRange notFound = { MMNotFound, 0 };

    if (str == nil || searchString == nil || str->cstring == nil)
        return notFound;

    const unsigned char *haystack = (const unsigned char *)str->cstring;
    size_t haystackLen = str->length;

    const unsigned char *needle = (const unsigned char *)searchString->cstring;
    size_t needleLen = searchString->length;

    size_t end = str->length;
    for (size_t i = 0; i <= haystackLen; i++) {
        if (memcmp(haystack + i, needle, needleLen) == 0) {
            return (MMRange){ i, needleLen };
        }
    }

    return notFound;
}

MMUInteger MMString_hash(MMString *str){
    if (!str || !str->cstring) return 0;

    MMUInteger h = 2166136261u;
    
    char *s =str->cstring;

    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

MMBool MMString_isEqualToString(MMString *str, MMString * aString){
    if (!str || !aString || !(str->cstring) || !(aString->cstring) || str->length!=aString->length)
    {
        return NO;
    }

    return strcmp(str->cstring, aString->cstring)==0;
}

MMUInteger MSString_lengthOfBytesUsingEncoding(MMString *str, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return str->length;   
}

MMString *MMString_stringByReplacingOccurrencesOfString(MMString *str, MMString *target, MMString *replacement){
    if (!str || !target || !target->cstring || target->length == 0) return NULL;

    size_t srcLen = str->length;
    size_t needleLen = target->length;
    size_t replLen = replacement ? replacement->length : 0;

    // First pass: count occurrences to compute new length
    size_t count = 0;
    for (size_t i = 0; i + needleLen <= srcLen; ) {
        if (memcmp(str->cstring + i, target->cstring, needleLen) == 0) {
            count++;
            i += needleLen;
        } else {
            i++;
        }
    }

    if (count == 0) {
        // No replacement needed: return copy of original
        MMString *copy = MMString_init(srcLen);
        if (!copy) return NULL;
        if (srcLen) memcpy(copy->cstring, str->cstring, srcLen);
        return copy;
    }

    size_t newLen = srcLen + count * (replLen - needleLen);
    MMString *out = MMString_init(newLen);
    if (!out) return NULL;

    // Second pass: build output
    size_t ri = 0;
    for (size_t i = 0; i < srcLen; ) {
        if (i + needleLen <= srcLen && memcmp(str->cstring + i, target->cstring, needleLen) == 0) {
            if (replLen > 0) memcpy(out->cstring + ri, replacement->cstring, replLen);
            ri += replLen;
            i += needleLen;
        } else {
            out->cstring[ri++] = str->cstring[i++];
        }
    }

    out->length = newLen;
    return out;
}

const char * MMString_fileSystemRepresentation(MMString *str){
    //TODO: implement true checks!
    return str->cstring;
}

//release
void MMString_release(MMString *str) {
    if (!str) return;
    free(str->cstring);
    free(str);
    str = nil;
}

MMBool MMString_getCString(MMString *str, char * buffer, MMUInteger maxBufferCount, MMStringEncoding encoding){
    //MMStringEncoding are not yet implemented!
    if (!str || !str->cstring || !buffer || maxBufferCount == 0) return NO;

    if (maxBufferCount > str->length) {
        buffer[0] = '\0';
        return NO;
    }

    memcpy(buffer, str->cstring, maxBufferCount);
    buffer[maxBufferCount] = '\0';

    return YES;
}


//-----MUTABLE STRING
MMMutableString *MMMutableString_initWithCString(const char *str){
    return (MMMutableString *)MMString_initWithCString(str);
}

void MMMutableString_release(MMMutableString *str){
    MMString_release((MMString *)str);
}




 
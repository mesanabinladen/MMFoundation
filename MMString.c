#include "MMTypes.h"

#include "MMString.h"
#include "MMData.h"
#include "MMError.h"
#include "MMArray.h"

/*Create an empty MMString */
MMString * MMString_init(size_t capacity){
    MMString *str = MM_init(MMTypeString);

    if (capacity < 0) {
        str->cString = nil;
    } 
    else{ //if capacity is zero, it means a nil terminated string!
        str->cString = malloc(capacity + 1);
        if (!str->cString) {
            free(str);
            return nil;
        }
        str->cString[capacity] = '\0';
    }  
    str->length = capacity;

    return str;
}

MMString *MMString_initWithCString(const char *cString) {
    if (!cString) return nil;

    size_t len = strlen(cString);
    MMString *newStr = MMString_init(len);
    if (!newStr) return nil;
    strcpy(newStr->cString, cString);
    return newStr;
}

MMString *MMString_initWithUTF8String(const char *nullTerminatedCString) {
    return MMString_initWithCString(nullTerminatedCString);
}

MMString *MMString_initWithFormat(const char *format, ...) {
    if (!format) return nil;

    va_list args;
    va_start(args, format);
    int needed = vsnprintf(nil, 0, format, args);
    va_end(args);

    if (needed < 0) return nil;

    MMString *newStr = MMString_init((size_t)needed);

    if (!newStr) return nil;

    va_start(args, format);
    vsnprintf(newStr->cString, (size_t)needed + 1, format, args);
    va_end(args);

    newStr->length = (size_t)needed;
    return newStr;
}

MMString *MMString_stringByDeletingLastPathComponent(const MMString *recv){
    if (!recv || !recv->cString) return nil;

    char *lastSlash = strrchr(recv->cString, MM_PATH_SEPARATOR);

    if (!lastSlash) {
        // No slash found, return an empty string
        MMString *newStr = MMString_init(0);
        return newStr;
    }

    size_t newLength = lastSlash - recv->cString;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strncpy(newStr->cString, recv->cString, newLength);

    return newStr;
}

MMString *MMString_stringByAppendingString(const MMString *recv, const MMString *append){
    if (!recv || !append) return nil;

    size_t newLength = recv->length + append->length;
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strcpy(newStr->cString, recv->cString);
    strcat(newStr->cString, append->cString);
    return newStr;
}

MMString * MMString_stringByAppendingPathComponent(const MMString *recv, MMString *str){
     if (!recv || !str) return nil;
    return MMString_stringByAppendingCPathComponent(recv, str->cString);
}

MMString * MMString_stringByAppendingCPathComponent(const MMString *recv, char *str){
     if (!recv || !str) return nil;

    size_t strLength = strlen(str);
    size_t newLength = recv->length + strLength + 1; //+ 1 for separator
    MMString *newStr = MMString_init(newLength);
    if (!newStr) return nil;

    strcpy(newStr->cString, recv->cString);

    char separator[2] = {MM_PATH_SEPARATOR, '\0'};
    strcat(newStr->cString, separator);

    strcat(newStr->cString, str);
    return newStr;
}

MMBool MMString_writeToFile(const MMString *recv, const MMString *path, MMBool atomically) {
    if (!recv || !path || !recv->cString || !path->cString) return NO;

    FILE *file = fopen(path->cString, atomically ? "w" : "w");
    if (!file) return NO;

    size_t written = fwrite(recv->cString, 1, recv->length, file);
    fclose(file);

    return written == recv->length;
}

const char *MMString_cStringUsingEncoding(const MMString *recv, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return recv->cString;
}

MMString * MMString_stringWithContentsOfFile(MMString * path, MMStringEncoding enc, MMError *error){
    if (error){
        printf("Error codes not implemented yet!");
        exit(1);
    }
    if (!path || !path->cString) return nil;

    MMData *data = MMData_initWithContentsOfFile(path);
    if (!data) return nil;

    //MMStringEncoding are not yet implemented!
    MMString *str = MMString_init(data->length);
    if (!str) {
        MMData_release(data);
        return nil;
    }

    if (str->length > 0) {
        memcpy(str->cString, data->buffer, str->length);
    }

    MMData_release(data);
    return str;
}

MMArray * MMString_componentsSeparatedByString(const MMString * recv, MMString * separator){
    if (!recv || !separator || !recv->cString || !separator->cString) return nil;
    if (separator->length == 0) return nil;

    MMMutableArray *result = MMMutableArray_init(); //
    if (!result) return nil;

    size_t start = 0;
    size_t sepLen = separator->length;
    while (start <= recv->length) {
        size_t found = MMNotFound;
        for (size_t i = start; i + sepLen <= recv->length; i++) {
            if (memcmp(recv->cString + i, separator->cString, sepLen) == 0) {
                found = i;
                break;
            }
        }

        size_t partLen;
        if (found == MMNotFound) {
            partLen = recv->length - start;
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
            memcpy(part->cString, recv->cString + start, partLen);
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
MMInteger MMString_integerValue(const MMString *recv) {
    char *end;
    if (recv->cString[0] == '\0' || recv->cString[0] == ' ')
        return 0;
    errno = 0;
    long l = strtol(recv->cString, &end, 10);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return 0;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return 0;
    if (*end != '\0')
        return 0;
    return l;
}

MMRange MMString_rangeOfString(const MMString *recv, MMString *searchString) {

    MMRange notFound = { MMNotFound, 0 };

    if (recv == nil || searchString == nil || recv->cString == nil) return notFound;

    return MMString_rangeOfCString(recv, searchString->cString);
}

MMRange MMString_rangeOfCString(const MMString *recv, const char * str){
    MMRange notFound = { MMNotFound, 0 };

    if (recv == nil || str == nil || recv->cString == nil) return notFound;

    const unsigned char *haystack = (const unsigned char *)recv->cString;
    size_t haystackLen = recv->length;

    const unsigned char *needle = (const unsigned char *)str;
    size_t needleLen = strlen(str);

    size_t end = recv->length;
    for (size_t i = 0; i <= haystackLen; i++) {
        if (memcmp(haystack + i, needle, needleLen) == 0) {
            return (MMRange){ i, needleLen };
        }
    }

    return notFound;
}


MMUInteger MMString_hash(const MMString *recv){
    if (!recv || !recv->cString) return 0;

    MMUInteger h = 2166136261u;
    
    char *s =recv->cString;

    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

MMBool MMString_isEqualToString(const MMString *recv, MMString * aString){
    if (!aString || !(aString->cString) || recv->length!=aString->length) return NO;

    return MMString_isEqualToCString(recv, aString->cString);
}

MMBool MMString_isEqualToCString(const MMString *recv, const char * str){
    if (!recv || !(recv->cString) || !str) return NO;

    return strcmp(recv->cString, str)==0;
}

MMUInteger MSString_lengthOfBytesUsingEncoding(const MMString *recv, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return recv->length;   
}

MMString *MMString_stringByReplacingOccurrencesOfString(const MMString *recv, MMString *target, MMString *replacement){
    if (!recv || !target || !target->cString || target->length == 0) return nil ;

    size_t srcLen = recv->length;
    size_t needleLen = target->length;
    size_t replLen = replacement ? replacement->length : 0;

    // First pass: count occurrences to compute new length
    size_t count = 0;
    for (size_t i = 0; i + needleLen <= srcLen; ) {
        if (memcmp(recv->cString + i, target->cString, needleLen) == 0) {
            count++;
            i += needleLen;
        } else {
            i++;
        }
    }

    if (count == 0) {
        // No replacement needed: return copy of original
        MMString *copy = MMString_init(srcLen);
        if (!copy) return nil ;
        if (srcLen) memcpy(copy->cString, recv->cString, srcLen);
        return copy;
    }

    size_t newLen = srcLen + count * (replLen - needleLen);
    MMString *out = MMString_init(newLen);
    if (!out) return nil ;

    // Second pass: build output
    size_t ri = 0;
    for (size_t i = 0; i < srcLen; ) {
        if (i + needleLen <= srcLen && memcmp(recv->cString + i, target->cString, needleLen) == 0) {
            if (replLen > 0) memcpy(out->cString + ri, replacement->cString, replLen);
            ri += replLen;
            i += needleLen;
        } else {
            out->cString[ri++] = recv->cString[i++];
        }
    }

    out->length = newLen;
    return out;
}

const char * MMString_fileSystemRepresentation(const MMString *recv){
    //TODO: implement true checks!
    return recv->cString;
}

MMBool MMString_getCString(const MMString *recv, char * buffer, MMUInteger maxBufferCount, MMStringEncoding encoding){
    //MMStringEncoding are not yet implemented!
    if (!recv || !recv->cString || !buffer || maxBufferCount == 0) return NO;

    if (maxBufferCount > recv->length) {
        buffer[0] = '\0';
        return NO;
    }

    memcpy(buffer, recv->cString, maxBufferCount);
    buffer[maxBufferCount] = '\0';

    return YES;
}

MMBool MMString_hasSuffix(const MMString * recv, MMString * str){
    if (recv == nil || str == nil || recv->cString == nil) return NO;
    
    size_t suffixLength = str->length;
    size_t haystackLen = recv->length;

    return strcmp(recv->cString + haystackLen - suffixLength, str->cString)==0;
}

MMString * MMString_substringFromIndex(const MMString * recv, MMUInteger from){
     if (recv == nil || recv->cString == nil) return nil;
    return MMString_initWithCString(&recv->cString[from]);
}

MMString * MMString_substringToIndex(const MMString * recv, MMUInteger to){
     if (recv == nil || recv->cString == nil) return nil;

    if (to > recv->length) to = recv->length;
    
    MMString *newStr = MMString_init(to);
    if (!newStr) return nil;
    
    if (to > 0) {
        memcpy(newStr->cString, recv->cString, to);
    }
    
    return newStr;
}

//str to num converter
void trimTrailingNewLine(char *str){
    size_t len = strlen(str);
    if (str[len-1] == '\n')
    {
        str[len-1]='\0';
    }
    
}

long MMString_longValue(const MMString * recv){
    if (!recv || !recv->cString) return 0;
    
    char *end;
    if (recv->cString[0] == '\0' || recv->cString[0] == ' ')
        return 0;
    errno = 0;

    trimTrailingNewLine(recv->cString);
    long d = strtol(recv->cString, &end, 10);
    
    if (errno == ERANGE)
        return 0;
    if (*end != '\0')
        return 0;
    
    return d;
}

long long MMString_longLongValue(const MMString * recv){
    if (!recv || !recv->cString) return 0;
    
    char *end;
    if (recv->cString[0] == '\0' || recv->cString[0] == ' ')
        return 0;
    errno = 0;

    trimTrailingNewLine(recv->cString);
    long long d = strtoll(recv->cString, &end, 10);
    
    if (errno == ERANGE)
        return 0;
    if (*end != '\0')
        return 0;
    
    return d;
}

double MMString_doubleValue(const MMString * recv){
    if (!recv || !recv->cString) return 0.0;
    
    char *end;
    if (recv->cString[0] == '\0' || recv->cString[0] == ' ')
        return 0.0;
    errno = 0;
    trimTrailingNewLine(recv->cString);
    double d = strtod(recv->cString, &end);
    
    if (errno == ERANGE)
        return 0.0;
    if (*end != '\0')
        return 0.0;
    
    return d;
}

MMString *MMString_copy(MMString * recv){
    if (!recv) return nil;
    
    return MMString_initWithCString(recv->cString);
}


//release
void MMString_release(MMString *recv) {
    if (!recv) return;
    free(recv->cString);
    free(recv);
    recv = nil;
}

//-----MUTABLE STRING
MMMutableString *MMMutableString_initWithCString(const char *str){
    return (MMMutableString *)MMString_initWithCString(str);
}

void MMMutableString_appendString(MMMutableString *recv, MMString * aString){
      if (!recv || !aString) return;

    size_t originalLength = recv->length;
    size_t newLength = recv->length + aString->length;//added string terminator!
    recv->cString = realloc(recv->cString, newLength + 1);      
        
    memcpy(&recv->cString[originalLength], aString->cString, aString->length);
    recv->cString[newLength]='\0';
    recv->length=newLength;
}

MMMutableString *MMMutableString_copy(MMMutableString * recv){
    return (MMMutableString *)MMString_copy(recv);
}
void MMMutableString_release(MMMutableString *recv){
    MMString_release((MMString *)recv);
}




 
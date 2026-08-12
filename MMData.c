#include "MMData.h"
#include "MMTypes.h"

MMData *MMData_initWithCapacity(size_t length){
    MMData *data = MM_init(MMTypeData);

    data->buffer = malloc(length);
    if (!data->buffer) {
        free(data);
        return NULL;
    }

    data->length = length;
    return data;
}

//Initializes a data object filled with a given number of bytes copied from a given buffer.
MMData *MMData_initWithBytes(const void *bytes, size_t length){
    MMData *data = MMData_initWithCapacity(length);
    memcpy(data->buffer, bytes, length);
    return data;

}
MMData *MMData_initWithContentsOfFile(MMString *path){
    FILE *file = fopen(MMString_cString(path), "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    MMData *data = MMData_initWithCapacity(length);
    fread(data->buffer, 1, length, file);
    fclose(file);

    return data;
}

MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return MMData_initWithBytes(str->cstring, str->length);
}

MMRange MMData_rangeOfData(MMData *data,
                           MMData *dataToFind,
                           MMDataSearchOptions mask,
                           MMRange searchRange) {

    MMRange notFound = { MMNotFound, 0 };

    if (data == NULL || dataToFind == NULL || data->buffer == NULL)
        return notFound;

    const unsigned char *haystack = (const unsigned char *)data->buffer;
    size_t haystackLen = data->length;

    const unsigned char *needle = (const unsigned char *)dataToFind->buffer;
    size_t needleLen = dataToFind->length;

    // Check for range validity
    if (searchRange.location > haystackLen)
        return notFound;
    if (searchRange.location + searchRange.length > haystackLen)
        searchRange.length = haystackLen - searchRange.location;

    // Special case: empty needle
    if (needleLen == 0) {
        if (mask & MMDataSearchBackwards) {
            // Apple always return the end of the search location
            return (MMRange){ searchRange.location + searchRange.length, 0 };
        }
        return (MMRange){ searchRange.location, 0 };
    }

    // Se il range da cercare è più piccolo del pattern → non trovato
    if (searchRange.length < needleLen)
        return notFound;

    // Backward search
    if (mask == MMDataSearchBackwards) {
        // Partiamo dalla posizione più a destra possibile
        size_t start = searchRange.location;
        size_t end   = searchRange.location + searchRange.length - needleLen; // inclusivo

        // Scorro da destra verso sinistra
        for (size_t i = end + 1; i > start; ) {
            i--;
            if (memcmp(haystack + i, needle, needleLen) == 0) {
                return (MMRange){ i, needleLen };
            }
        }
        return notFound;
    }
    else{
        size_t end = searchRange.location + searchRange.length - needleLen;
        for (size_t i = searchRange.location; i <= end; i++) {
            if (memcmp(haystack + i, needle, needleLen) == 0) {
                return (MMRange){ i, needleLen };
            }
        }
    }
    return notFound;
}

void MMData_release(MMData *data) {
    if (!data) return;
    free(data->buffer);
    free(data);
    data = nil;
}

MMMutableData *MMMutableData_initWithCapacity(size_t size){
    return (MMMutableData *)MMData_initWithCapacity(size);

}
MMMutableData *MMMutableData_initWithBytes(const void *bytes, size_t length){
    return (MMMutableData *)MMData_initWithBytes(bytes, length);

}
MMMutableData *MMMutableData_initWithContentsOfFile(MMString *path){
    return (MMMutableData *)MMData_initWithContentsOfFile(path);
}

void MMMutableData_appendBytes(MMMutableData * data, const void * bytes, MMUInteger length){
    if (!data || !bytes || !length) return;

    size_t originalLength = data->length;
    size_t newLength = data->length + length;
    data->buffer = realloc(data->buffer, newLength);      
    
    unsigned char *dst = (unsigned char *)data->buffer;
    memcpy(dst + originalLength, bytes, length);

}

void MMMutableData_appendData(MMMutableData * data, MMData * other){
    MMMutableData_appendBytes(data, other->buffer, other->length);
}


void MMMutableData_release(MMMutableData *data) {
    MMData_release(data);
}
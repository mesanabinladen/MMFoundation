#include "MMData.h"
#include "MMTypes.h"

MMData *MMData_initWithCapacity(size_t length){
    MMData *data = MM_init(MMTypeData);

    data->bytes = malloc(length);
    if (!data->bytes) {
        free(data);
        return NULL;
    }

    data->length = length;
    return data;
}

//Initializes a data object filled with a given number of bytes copied from a given buffer.
MMData *MMData_initWithBytes(const void *bytes, size_t length){
    MMData *data = MMData_initWithCapacity(length);
    memcpy(data->bytes, bytes, length);
    return data;

}
MMData *MMData_initWithContentsOfFile(const MMString *path){
    FILE *file = fopen(path->cString, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    MMData *data = MMData_initWithCapacity(length);
    fread(data->bytes, 1, length, file);
    fclose(file);

    return data;
}

void MMData_getBytes(const MMData *recv, void * buffer , MMUInteger length){
    if (!recv || !buffer) return; 
    memcpy(buffer, recv->bytes, length);
}


MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc){
    //MMStringEncoding are not yet implemented!
    return MMData_initWithBytes(str->cString, str->length);
}

MMRange MMData_rangeOfData(const MMData *recv, MMData *dataToFind, MMDataSearchOptions mask, MMRange searchRange) {
    MMRange notFound = { MMNotFound, 0 };

    if (recv == NULL || dataToFind == NULL || recv->bytes == NULL)
        return notFound;

    const unsigned char *haystack = (const unsigned char *)recv->bytes;
    size_t haystackLen = recv->length;

    const unsigned char *needle = (const unsigned char *)dataToFind->bytes;
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

MMBool MMData_writeToFile(const MMData *recv, const MMString *path, MMBool useAuxiliaryFile) {
    if (!recv || !path || !recv->bytes || !path->cString) return NO;

    FILE *file = fopen(path->cString, useAuxiliaryFile ? "w" : "w");
    if (!file) return NO;

    size_t written = fwrite(recv->bytes, 1, recv->length, file);
    fclose(file);

    return written == recv->length;
}

MMData *MMData_copy(MMData * recv){
    if (!recv) return nil;
    
    return MMData_initWithBytes(recv->bytes, recv->length);
}

void MMData_release(MMData *recv) {
    if (!recv) return;
    free(recv->bytes);
    free(recv);
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

void MMutableData_getBytes(const MMMutableData *recv, void * buffer , MMUInteger length){
    MMData_getBytes((MMData *)recv, buffer, length);
}

void MMMutableData_appendBytes(MMMutableData * recv, const void * bytes, MMUInteger length){
    if (!recv || !bytes || !length) return;

    size_t originalLength = recv->length;
    size_t newLength = recv->length + length;
    recv->bytes = realloc(recv->bytes, newLength);      
    
    unsigned char *dst = (unsigned char *)recv->bytes;
    memcpy(dst + originalLength, bytes, length);

}

void MMMutableData_appendData(MMMutableData * recv, MMData * other){
    MMMutableData_appendBytes(recv, other->bytes, other->length);
}

MMBool MMMutableData_writeToFile(const MMMutableData *recv, const MMString *path, MMBool useAuxiliaryFile){
    return MMData_writeToFile((const MMData *)recv, path, useAuxiliaryFile);
}
MMMutableData *MMMutableData_copy(MMMutableData * recv){
    return (MMMutableData *)MMData_copy((MMData*)recv);
}

void MMMutableData_release(MMMutableData *recv) {
    MMData_release((MMData*)recv);
}
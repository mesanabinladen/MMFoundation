#include "MMData.h"
#include "MMTypes.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#endif

MMData *MMData_initWithCapacity(size_t length){
    MMData *data = MM_init(MMTypeData);

    data->bytes = malloc(length ? length : 1);
    if (!data->bytes) {
        free(data);
        return NULL;
    }

    data->length = length;
    data->isMemoryMapped = NO;
    return data;
}

//Initializes a data object filled with a given number of bytes copied from a given buffer.
MMData *MMData_initWithBytes(const void *bytes, size_t length){
    if (!bytes && length > 0) return NULL;

    MMData *data = MMData_initWithCapacity(length);
    if (!data) return NULL;
    if (length > 0) memcpy(data->bytes, bytes, length);
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

MMData *MMData_dataWithContentsOfMappedFile(const MMString *path){
    if (!path || !path->cString) return NULL;

    void *bytes = NULL;
    size_t length = 0;
    MMBool isMemoryMapped = NO;

#if defined(_WIN32) || defined(_WIN64)
    HANDLE file = CreateFileA(path->cString, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) return NULL;

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(file, &fileSize) || fileSize.QuadPart < 0 || (uint64_t)fileSize.QuadPart > SIZE_MAX){
        CloseHandle(file);
        return NULL;
    }
    length = (size_t)fileSize.QuadPart;

    if (length > 0){
        HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);
        if (!mapping){
            CloseHandle(file);
            return NULL;
        }

        bytes = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
        CloseHandle(mapping);
        if (!bytes){
            CloseHandle(file);
            return NULL;
        }
        isMemoryMapped = YES;
    }
    CloseHandle(file);
#else
    int fd = open(path->cString, O_RDONLY);
    if (fd < 0) return NULL;

    struct stat fileInfo;
    if (fstat(fd, &fileInfo) != 0 || fileInfo.st_size < 0 || (uintmax_t)fileInfo.st_size > SIZE_MAX){
        close(fd);
        return NULL;
    }
    length = (size_t)fileInfo.st_size;

    if (length > 0){
        bytes = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
        if (bytes == MAP_FAILED) return NULL;
        isMemoryMapped = YES;
    }
    else{
        close(fd);
    }
#endif

    MMData *data = MM_init(MMTypeData);
    data->bytes = bytes;
    data->length = length;
    data->isMemoryMapped = isMemoryMapped;
    return data;
}

void MMData_getBytes(const MMData *recv, void * buffer , MMUInteger length){
    if (!recv || !buffer) return; 
    memcpy(buffer, recv->bytes, length);
}

void MMData_getBytesFromRange(const MMData *recv, void * buffer , MMRange range){
    if (!recv || !buffer) return; 
    memcpy(buffer, &((char *)recv->bytes)[range.location], range.length);
}


MMData *MMData_dataUsingEncoding(const MMString * str, MMStringEncoding enc){
    (void)enc;
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

MMData *MMData_subdataWithRange(const MMData *recv, MMRange range){
    if (!recv || !recv->bytes) return NULL;
    if (range.location + range.length > recv->length) return NULL;

    MMData *subdata = MMData_initWithCapacity(range.length);
    memcpy(subdata->bytes, (unsigned char *)recv->bytes + range.location, range.length);
    return subdata;
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
    if (recv->isMemoryMapped){
#if defined(_WIN32) || defined(_WIN64)
        UnmapViewOfFile(recv->bytes);
#else
        munmap(recv->bytes, recv->length);
#endif
    }
    else{
        free(recv->bytes);
    }
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

void MMMutableData_getBytes(const MMMutableData *recv, void * buffer , MMUInteger length){
    MMData_getBytes((MMData *)recv, buffer, length);
}

void MMMutableData_getBytesFromRange(const MMMutableData *recv, void * buffer , MMRange range){
    MMData_getBytesFromRange((MMData *)recv, buffer, range);
}

MMData *MMMutableData_subdataWithRange(const MMMutableData *recv, MMRange range){
    return MMData_subdataWithRange((const MMData *)recv, range);
}

void MMMutableData_appendBytes(MMMutableData * recv, const void * bytes, MMUInteger length){
    if (!recv || !bytes || !length) return;

    size_t originalLength = recv->length;
    recv->length  = recv->length + length;
    recv->bytes = realloc(recv->bytes, recv->length );      
 
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
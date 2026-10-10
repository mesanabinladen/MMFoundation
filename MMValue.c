#include "MMValue.h"
#include "MMMemoryManagement.h"

MMValue *MMValue_valueWithBytes(const void *value, size_t size){
    if (!value || size == 0) return nil;

    MMValue *result = MM_init(MMTypeValue);
    result->bytes = nil;
    result->size = 0;
    result->bytes = malloc(size);
    if (!result->bytes) {
        MM_release(result);
        return nil;
    }

    memcpy(result->bytes, value, size);
    result->size = size;
    return result;
}

MMValue *MMValue_valueWithPointer(const void *pointer){
    return MMValue_valueWithBytes(&pointer, sizeof(pointer));
}

MMBool MMValue_getValue(const MMValue *recv, void *buffer, size_t bufferSize){
    if (!recv || !buffer || bufferSize < recv->size) return NO;
    memcpy(buffer, recv->bytes, recv->size);
    return YES;
}

void *MMValue_pointerValue(const MMValue *recv){
    void *pointer = nil;
    if (!recv || recv->size != sizeof(pointer)) return nil;
    memcpy(&pointer, recv->bytes, sizeof(pointer));
    return pointer;
}

MMValue *MMValue_copy(const MMValue *recv){
    if (!recv) return nil;
    MMValue *copy = MM_init(MMTypeValue);
    copy->bytes = nil;
    copy->size = 0;
    copy->bytes = malloc(recv->size);
    if (!copy->bytes) {
        MM_release(copy);
        return nil;
    }
    memcpy(copy->bytes, recv->bytes, recv->size);
    copy->size = recv->size;
    return copy;
}

void MMValue_release(MMValue *recv){
    if (!recv) return;
    free(recv->bytes);
    recv->bytes = nil;
    recv->size = 0;
    free(recv);
}

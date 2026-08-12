#include "MMFileHandle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <unistd.h>
#endif

//private function to create a file if it doesn't exist
MMFileHandle *getFileHandle(const MMString *path, int flags, int mode) {
     if (!path || !path->cstring) return NULL;

    int fd = open(path->cstring, flags, mode);
    if (fd < 0) {
        return NULL;
    }

    MMFileHandle *handle = (MMFileHandle*)malloc(sizeof(MMFileHandle));
    if (!handle) {
        close(fd);
        return NULL;
    }
    handle->type = MMTypeFileHandle;
    handle->retainCount = 1;
    handle->fd = fd;
    return handle;
}

//--------------------------------
MMFileHandle *MMFileHandle_fileHandleForReadingAtPath(const MMString *path){

    return getFileHandle(path, O_RDONLY, 0);
}

MMFileHandle *MMFileHandle_fileHandleForWritingAtPath(const MMString *path){
    
    return getFileHandle(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

MMFileHandle *MMFileHandle_fileHandleForUpdatingAtPath(const MMString *path){
       
    return getFileHandle(path, O_RDWR, 0);
}

void MMFileHandle_seekToFileOffset(MMFileHandle *handle, off_t offset) {
    lseek(handle->fd, offset, SEEK_SET);
}

off_t MMFileHandle_seekToEndOfFile(MMFileHandle *handle) {
     return lseek(handle->fd, 0, SEEK_END);
}

MMData *MMFileHandle_readDataOfLength(MMFileHandle *handle, size_t length){
    
    char * buffer=malloc(length); //I have to use malloc (heap) instead of stack because stack is not big enough
    read(handle->fd, buffer, length);
    MMData *d = MMData_initWithBytes(buffer, length);
    free(buffer);
    return d;
}

MMData *MMfileHandle_readDataToEndOfFile(MMFileHandle *handle){
    size_t size = MMFileHandle_seekToEndOfFile(handle);
    return MMFileHandle_readDataOfLength(handle, size);
}

MMData * MMFileHandle_availableData(MMFileHandle *handle) {
    off_t currentOffset = lseek(handle->fd, 0, SEEK_CUR);
    off_t endOffset = lseek(handle->fd, 0, SEEK_END);
    lseek(handle->fd, currentOffset, SEEK_SET); // Restore the original offset

    size_t availableSize = (size_t)(endOffset - currentOffset);

    MMData *d = MMData_initWithCapacity(availableSize);

    read(handle->fd, d->buffer, availableSize);

    return d;
}


ssize_t MMFileHandle_writeData(MMFileHandle *handle, const MMData *data) {
    return write(handle->fd, data->buffer, data->length);
}

void MMFileHandle_truncateFileAtOffset(MMFileHandle *handle, off_t offset){     
    if (!handle) return;
    ftruncate(handle->fd, offset);
}   

off_t MMFileHandle_offsetInFile(MMFileHandle *handle) {
    return lseek(handle->fd, 0, SEEK_CUR);
}

void MMFileHandle_synchronizeFile(MMFileHandle *handle) {
    if (handle) {
#if defined(_WIN32) || defined(_WIN64)
        _commit(handle->fd);
#else
        fsync(handle->fd);
#endif
    }
}

void MMFileHandle_closeFile(MMFileHandle *handle) {
    if (handle) {
        close(handle->fd);
        free(handle);
    }
    handle = nil;
}
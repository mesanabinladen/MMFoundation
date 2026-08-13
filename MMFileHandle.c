#include "MMFileHandle.h"
#include "MMTypes.h"

//private function to create a file if it doesn't exist
MMFileHandle *getFileHandle(const MMString *path, int flags, int mode) {
     if (!path || !path->cString) return NULL;

    int fd = open(path->cString, flags, mode);
    if (fd < 0) {
        return NULL;
    }

    MMFileHandle *handle = MM_init(MMTypeFileHandle);
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

void MMFileHandle_seekToFileOffset(const MMFileHandle *recv, off_t offset) {
    lseek(recv->fd, offset, SEEK_SET);
}

off_t MMFileHandle_seekToEndOfFile(const MMFileHandle *recv) {
     return lseek(recv->fd, 0, SEEK_END);
}

MMData *MMFileHandle_readDataOfLength(const MMFileHandle *recv, size_t length){
    char * buffer=malloc(length); //I have to use malloc (heap) instead of stack because stack is not big enough
    read(recv->fd, buffer, length);
    MMData *d = MMData_initWithBytes(buffer, length);
    free(buffer);
    return d;
}

MMData *MMfileHandle_readDataToEndOfFile(const MMFileHandle *recv){
    size_t size = MMFileHandle_seekToEndOfFile(recv);
    return MMFileHandle_readDataOfLength(recv, size);
}

MMData * MMFileHandle_availableData(const MMFileHandle *recv) {
    off_t currentOffset = lseek(recv->fd, 0, SEEK_CUR);
    off_t endOffset = lseek(recv->fd, 0, SEEK_END);
    lseek(recv->fd, currentOffset, SEEK_SET); // Restore the original offset

    size_t availableSize = (size_t)(endOffset - currentOffset);

    MMData *d = MMData_initWithCapacity(availableSize);

    read(recv->fd, d->buffer, availableSize);

    return d;
}


ssize_t MMFileHandle_writeData(MMFileHandle *recv, const MMData *data) {
    return write(recv->fd, data->buffer, data->length);
}

void MMFileHandle_truncateFileAtOffset(MMFileHandle *recv, off_t offset){     
    if (!recv) return;
    ftruncate(recv->fd, offset);
}   

off_t MMFileHandle_offsetInFile(const MMFileHandle *recv) {
    return lseek(recv->fd, 0, SEEK_CUR);
}

void MMFileHandle_synchronizeFile(MMFileHandle *recv) {
    if (recv) {
#if defined(_WIN32) || defined(_WIN64)
        _commit(recv->fd);
#else
        fsync(recv->fd);
#endif
    }
}

void MMFileHandle_closeFile(MMFileHandle *recv) {
    if (recv) {
        close(recv->fd);
        free(recv);
    }
    recv = nil;
}
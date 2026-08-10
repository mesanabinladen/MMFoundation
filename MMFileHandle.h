#ifndef MMFILEHANDLE_H
#define MMFILEHANDLE_H

#include "MMString.h"
#include "MMData.h"

#include <fcntl.h>
#include <unistd.h>

typedef struct MMFileHandle {
    int fd; // File descriptor
} MMFileHandle;

MMFileHandle *MMFileHandle_fileHandleForReadingAtPath(const MMString *path);
MMFileHandle *MMFileHandle_fileHandleForWritingAtPath(const MMString *path);
MMFileHandle *MMFileHandle_fileHandleForUpdatingAtPath(const MMString *path);

void MMFileHandle_seekToFileOffset(MMFileHandle *handle, off_t offset);
off_t MMFileHandle_seekToEndOfFile(MMFileHandle *handle);

MMData *MMFileHandle_readDataOfLength(MMFileHandle *handle, size_t length);
ssize_t MMFileHandle_availableData(MMFileHandle *handle, void *buffer);
ssize_t MMFileHandle_writeData(MMFileHandle *handle, const void *buffer, size_t size);
void MMFileHandle_truncateAtFileOffset(MMFileHandle *handle, off_t offset);

off_t MMFileHandle_offsetInFile(MMFileHandle *handle);

void MMFileHandle_close(MMFileHandle *handle);
#endif /* MMFILEHANDLE_H */
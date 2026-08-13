#ifndef MMFILEHANDLE_H
#define MMFILEHANDLE_H

#include "MMString.h"
#include "MMData.h"

#include <fcntl.h>
#include <unistd.h>

typedef struct MMFileHandle {
    int type;
    int retainCount;
    //-------------
    int fd; // File descriptor
} MMFileHandle;

MMFileHandle *MMFileHandle_fileHandleForReadingAtPath(const MMString *path);
MMFileHandle *MMFileHandle_fileHandleForWritingAtPath(const MMString *path);
MMFileHandle *MMFileHandle_fileHandleForUpdatingAtPath(const MMString *path);
void MMFileHandle_seekToFileOffset(const MMFileHandle *recv, off_t offset);
off_t MMFileHandle_seekToEndOfFile(const MMFileHandle *recv);
MMData *MMFileHandle_readDataOfLength(const MMFileHandle *recv, size_t length);
MMData *MMfileHandle_readDataToEndOfFile(const MMFileHandle *recv);
MMData * MMFileHandle_availableData(const MMFileHandle *recv);
ssize_t MMFileHandle_writeData(MMFileHandle *recv, const MMData *data);
void MMFileHandle_truncateFileAtOffset(MMFileHandle *recv, off_t offset);
off_t MMFileHandle_offsetInFile(const MMFileHandle *recv);
void MMFileHandle_synchronizeFile(MMFileHandle *recv);
void MMFileHandle_closeFile(MMFileHandle *recv);
void MMFileHandle_release(MMFileHandle *recv);

#endif /*MMFILEHANDLE_H*/
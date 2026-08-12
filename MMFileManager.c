#include "MMFileManager.h"
#include "MMTypes.h"

int MMFileManager_fileExistsAtPath(const MMString *path) {
    struct stat st;
    return (stat(path->cstring, &st) == 0);
}

int MMFileManager_createFileAtPath(const MMString *path, MMData * content, void * attributes) {
    if (content!=nil)
    {
        printf("Error create file with content with MMFileManager is not yet available!");
        exit(1);
    }
    if (attributes!=nil)
    {
        printf("Error create file with attributes with MMFileManager is not yet available!");
        exit(1);
    }
    int fd = open(path->cstring, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd < 0) return 0;
    close(fd);
    return 1;
}

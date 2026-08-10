#ifndef MMFILEMANAGER_H
#define MMFILEMANAGER_H
#include "MMString.h"
#include "MMData.h"

int MMFileManager_fileExistsAtPath(const MMString *path);
int MMFileManager_createFileAtPath(const MMString *path, MMData * content, void * attributes);

#endif
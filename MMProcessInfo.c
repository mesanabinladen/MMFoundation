#include "MMProcessInfo.h"
#include "MMTypes.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

MMProcessInfo *MMProcessInfo_init(void){
    MMProcessInfo *processInfo = MM_init(MMTypeProcessInfo);

#if defined(_WIN32) || defined(_WIN64)
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    processInfo->processorCount = systemInfo.dwNumberOfProcessors;
#else
    long processorCount = sysconf(_SC_NPROCESSORS_ONLN);
    processInfo->processorCount = processorCount > 0 ? (unsigned int)processorCount : 0;
#endif

    if (processInfo->processorCount == 0) processInfo->processorCount = 1;
    return processInfo;
}

MMProcessInfo *MMProcessInfo_copy(const MMProcessInfo *recv){
    if (!recv) return NULL;

    MMProcessInfo *copy = MM_init(MMTypeProcessInfo);
    copy->processorCount = recv->processorCount;
    return copy;
}

void MMProcessInfo_release(MMProcessInfo *recv){
    free(recv);
}
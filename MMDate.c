#include "MMDate.h"
#include <stdlib.h>
#include <sys/time.h>

MMDate * MMDate_init(){
    MMDate *date = (MMDate *)malloc(sizeof(MMDate));
    if (!date) {
        return NULL;
    }

    struct timeval now;
    gettimeofday(&now, NULL);
    date->timeIntervalSinceReferenceDate = (MMTimeInterval)((long long)now.tv_sec * 1000LL + (long long)now.tv_usec / 1000LL);
    return date;
}

MMTimeInterval MMDate_timeIntervalSinceDate(MMDate *self, MMDate *date){
    return (self->timeIntervalSinceReferenceDate - date->timeIntervalSinceReferenceDate) / 1000;
}

//release
void MMDate_release(MMDate *date){
    if (!date) {
        return;
    }
    free(date);
}
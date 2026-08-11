#include <stdlib.h>
#include <sys/time.h>

#include "MMDate.h"
#include "MMTypes.h"

MMDate * MMDate_init(){
    MMDate *date = (MMDate *)malloc(sizeof(MMDate));
    if (!date) {
        return NULL;
    }

    struct timeval now;
    gettimeofday(&now, NULL);
    date->type = MMTypeDate;
    date->retainCount = 1;
    date->timeIntervalSinceReferenceDate = (MMTimeInterval)((long long)now.tv_sec * 1000LL + (long long)now.tv_usec / 1000LL);
    return date;
}

MMTimeInterval MMDate_timeIntervalSinceDate(MMDate *date, MMDate *anotherDate){
    return (date->timeIntervalSinceReferenceDate - anotherDate->timeIntervalSinceReferenceDate) / 1000;
}

//release
void MMDate_release(MMDate *date){
    if (!date) {
        return;
    }
    free(date);
    date = nil;
}
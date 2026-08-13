#include "MMDate.h"
#include "MMTypes.h"

MMDate * MMDate_init(){
    MMDate *date = MM_init(MMTypeDate);

    struct timeval now;
    gettimeofday(&now, NULL);
    date->timeIntervalSinceReferenceDate = (MMTimeInterval)((long long)now.tv_sec * 1000LL + (long long)now.tv_usec / 1000LL);

    return date;
}

MMTimeInterval MMDate_timeIntervalSinceDate(const MMDate *recv, MMDate *anotherDate){
    return (recv->timeIntervalSinceReferenceDate - anotherDate->timeIntervalSinceReferenceDate) / 1000;
}

//release
void MMDate_release(MMDate *recv){
    if (!recv) return;
    free(recv);
}
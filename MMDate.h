#ifndef MMDATE_H
#define MMDATE_H

typedef double MMTimeInterval ;

typedef struct MMDate {
    int type;
    int retainCount;
    //-------------
    MMTimeInterval timeIntervalSinceReferenceDate; // Time interval in milliseconds since UNIX epoch (Jan 1, 1970)
    // Date properties
} MMDate;

MMDate * MMDate_init();
MMTimeInterval MMDate_timeIntervalSinceDate(const MMDate *recv, MMDate *anotherDate);
void MMDate_release(MMDate * recv);

#endif /*MMDATE_H*/
#ifndef MMNUMBER_h
#define MMNUMBER_h

typedef struct MMNumber {
    int type;
    int retainCount;
    //------------- 
    unsigned int ui;
} MMNumber;


MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value);

void MMNumber_release(MMNumber * n);

#endif
#ifndef MMNUMBER_h
#define MMNUMBER_h

#include "MMTypes.h"

typedef struct MMNumber {
    int type;
    int retainCount;
    //------------- 
    union
    {
        MMBool boolValue;
        char charValue;
        short shortValue;
        int intValue;
        long longValue;
        long long longLongValue;
        unsigned char unsignedCharValue;
        unsigned short unsignedShortValue;
        unsigned int unsignedIntValue;
        unsigned long unsignedLongValue;
        unsigned long long unsignedLongLongValue;
        float floatValue;
        double doubleValue;
    };
} MMNumber;

MMNumber * MMNumber_numberWithBool(MMBool value);
MMNumber * MMNumber_numberWithChar(char value);
MMNumber * MMNumber_numberWithShort(short value);
MMNumber * MMNumber_numberWithInt(int value);
MMNumber * MMNumber_numberWithLong(long value);
MMNumber * MMNumber_numberWithLongLong(long long value);
MMNumber * MMNumber_numberWithUnsignedChar(unsigned char value);
MMNumber * MMNumber_numberWithUnsignedShort(unsigned short value);
MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value);
MMNumber * MMNumber_numberWithUnsignedLong(unsigned long value);
MMNumber * MMNumber_numberWithUnsignedLongLong(unsigned long long value);
MMNumber * MMNumber_numberWithFloat(float value);
MMNumber * MMNumber_numberWithDouble(double value);
void MMNumber_release(MMNumber * n);

#endif /*MMNUMBER_h*/
#include "MMNumber.h"
#include "MMTypes.h"

MMNumber * MMNumber_numberWithBool(MMBool value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->boolValue = value;
    return n;
}

MMNumber * MMNumber_numberWithChar(char value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->charValue = value;
    return n;
}

MMNumber * MMNumber_numberWithShort(short value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->shortValue = value;
    return n;
}

MMNumber * MMNumber_numberWithInt(int value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->intValue = value;
    return n;
}

MMNumber * MMNumber_numberWithLong(long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->longValue = value;
    return n;
}

MMNumber * MMNumber_numberWithLongLong(long long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->longLongValue = value;
    return n;
}

MMNumber * MMNumber_numberWithUnsignedChar(unsigned char value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedCharValue = value;
    return n;
}

MMNumber * MMNumber_numberWithUnsignedShort(unsigned short value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedShortValue = value;
    return n;
}

MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedIntValue = value;
    return n;
}

MMNumber * MMNumber_numberWithUnsignedLong(unsigned long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedLongValue = value;
    return n;
}

MMNumber * MMNumber_numberWithUnsignedLongLong(unsigned long long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedLongLongValue = value;
    return n;
}

MMNumber * MMNumber_numberWithFloat(float value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->floatValue = value;
    return n;
}

MMNumber * MMNumber_numberWithDouble(double value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->doubleValue = value;
    return n;
}



void MMNumber_release(MMNumber * recv){
    if (!recv) return;
    free(recv);
    recv = nil;
}

#include "MMNumber.h"
#include "MMTypes.h"

static void assignFloat(MMNumber * n, float f){
    n->floatValue  = f;
    n->doubleValue = f;
}

MMNumber * MMNumber_numberWithBool(MMBool value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->boolValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithChar(char value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->charValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithShort(short value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->shortValue = value; 
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithInt(int value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->intValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithLong(long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->longValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithLongLong(long long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->longLongValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithUnsignedChar(unsigned char value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedCharValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithUnsignedShort(unsigned short value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedShortValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithUnsignedInt(unsigned int value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedIntValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithUnsignedLong(unsigned long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedLongValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithUnsignedLongLong(unsigned long long value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->unsignedLongLongValue = value;
    assignFloat(n, (float)value);
    return n;
}

MMNumber * MMNumber_numberWithFloat(float value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->floatValue = value;
    n->intValue = (int)value;
    return n;
}

MMNumber * MMNumber_numberWithDouble(double value){
    MMNumber * n = MM_init(MMTypeNumber);
    n->doubleValue = value;
    n->intValue = (int)value;
    return n;
}

MMString * MMNumber_stringValue(MMNumber *recv){
    if (recv == nil) return nil;

    MMString *str = MMString_initWithFormat("%f", recv->doubleValue);
    return str;
}

void MMNumber_release(MMNumber * recv){
    if (!recv) return;
    free(recv);
}

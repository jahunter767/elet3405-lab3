#ifndef ext_int_h
#define ext_int_h

#include "default.h"

typedef struct ExtIntStruct{
    // MCUCR Reg bits
    unsigned char intSenseCtrl0 :2;
    unsigned char intSenseCtrl1 :2;

    // MCUCSR Reg bits
    unsigned char intSenseCtrl2 :1;
} ExtInt;

void initExtInt(ExtInt* conf);
void enableExtInt(unsigned char i);
void disableExtInt(unsigned char i);
void clearExtIntFlags(unsigned char f);

#endif
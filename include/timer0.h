#ifndef timer0_h
#define timer0_h

#include "default.h"
#include <avr/interrupt.h>

typedef struct Timer0Struct{
    unsigned int prescaler;

    unsigned char forceOutputComp :1;
    unsigned char compOutputMode :2;
    unsigned char waveformGenMode :2;
    unsigned char clockSelect :3;

    unsigned char outputCompReg;
    unsigned char timerCounterReg;
} Timer0;

ISR(TIMER0_OVF_vect);

void initTimer0(Timer0* conf);
void enableTimer0OutputCompInt();
void disableTimer0OutputCompInt();
void enableTimer0OvfInt();
void disableTimer0OvfInt();
void clearTimer0IntFlags();

void setTCNT0(Timer0* conf);
void startTimer0(Timer0* conf);
void stopTimer0();
unsigned long getTicks0();
double getTimeUS0(Timer0* conf);
double getFreq0(Timer0* conf);

#endif
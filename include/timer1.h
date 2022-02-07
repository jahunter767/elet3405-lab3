#ifndef timer1_h
#define timer1_h

#include "default.h"
#include <avr/interrupt.h>

typedef struct Timer1Struct{
    unsigned int prescaler;

    unsigned char compOutputModeA :2;
    unsigned char compOutputModeB :2;
    unsigned char forceOutputCompA :1;
    unsigned char forceOutputCompB :1;

    unsigned char waveformGenMode :4;
    unsigned char inputCapNoiseCanceler :1;
    unsigned char inputCapEdgeSel :1;
    unsigned char clockSelect :3;

    unsigned int outputCompAReg;
    unsigned int outputCompBReg;
    unsigned int timerCounterReg;
} Timer1;

ISR(TIMER1_OVF_vect);

void initTimer1(Timer1* conf);

void enableTimer1InputCapInt();
void disableTimer1InputCapInt();

void enableTimer1OutputCompAInt();
void disableTimer1OutputCompAInt();

void enableTimer1OutputCompBInt();
void disableTimer1OutputCompBInt();

void enableTimer1OvfInt();
void disableTimer1OvfInt();

void clearTimer1IntFlags();

void setTCNT1(Timer1* conf);
void startTimer1(Timer1* conf);
void stopTimer1();
unsigned long getTicks1();
double getTimeUS1(Timer1* conf);
double getFreq1(Timer1* conf);

#endif
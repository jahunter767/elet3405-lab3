#include "../include/timer1.h"

volatile unsigned int ovfCount1 = 0;

ISR(TIMER1_OVF_vect){
    ovfCount1 += 1;
} // End-TIMER1_OVF ISR

void initTimer1(Timer1* conf){
    OCR1AH = (conf->outputCompAReg >> 8);
    OCR1AL = (conf->outputCompAReg & 0x00FF);

    OCR1BH = (conf->outputCompBReg >> 8);
    OCR1BL = (conf->outputCompBReg & 0x00FF);

    setTCNT1(conf);

    switch (conf->clockSelect){
        case (1):
            conf->prescaler = 1;
            break;

        case (2):
            conf->prescaler = 8;
            break;

        case (3):
            conf->prescaler = 64;
            break;

        case (4):
            conf->prescaler = 256;
            break;

        case (5):
            conf->prescaler = 1024;
            break;
    } // End-switch

    TCCR1A = (conf->compOutputModeA << COM1A0) |
             (conf->compOutputModeB << COM1B0) |
             (conf->forceOutputCompA << FOC1A) |
             (conf->forceOutputCompB << FOC1B) |
             (((conf->waveformGenMode & 0x02) >> 1) << WGM11) |
             ((conf->waveformGenMode & 0x01) << WGM10);

    TCCR1B = (conf->inputCapNoiseCanceler << ICNC1) |
             (conf->inputCapEdgeSel << ICES1) |
             (((conf->waveformGenMode & 0x08) >> 3) << WGM13) |
             (((conf->waveformGenMode & 0x04) >> 2) << WGM12);
} // End-initTimer1

void enableTimer1InputCapInt(){
    TIMSK |= (1 << TICIE1);
} // End-enableTimer1InputCapInt

void disableTimer1InputCapInt(){
    TIMSK &= ~(1 << TICIE1);
} // End-disableTimer1InputCapInt

void enableTimer1OutputCompAInt(){
    TIMSK |= (1 << OCIE1A);
} // End-enableTimer1OutputCompAInt

void disableTimer1OutputCompAInt(){
    TIMSK &= ~(1 << OCIE1A);
} // End-disableTimer1OutputCompAInt

void enableTimer1OutputCompBInt(){
    TIMSK |= (1 << OCIE1B);
} // End-enableTimer1OutputCompBInt

void disableTimer1OutputCompBInt(){
    TIMSK &= ~(1 << OCIE1B);
} // End-disableTimer1OutputCompBInt

void enableTimer1OvfInt(){
    TIMSK |= (1 << TOIE1);
} // End-enableTimer1OvfInt

void disableTimer1OvfInt(){
    TIMSK &= ~(1 << TOIE1);
} // End-disableTimer1OvfInt


void clearTimer1IntFlags(){
    TIFR |= (1 << ICF1) | (1 << OCF1A) | (1 << OCF1B) | (1 << TOV1);
} // End-clearTimer1IntFlags

void setTCNT1(Timer1* conf){
    TCNT1H = (conf->timerCounterReg >> 8);
    TCNT1L = (conf->timerCounterReg & 0x00FF);
} // End-setTCNT1

void startTimer1(Timer1* conf){
    ovfCount1 = 0;
    setTCNT1(conf);
    TCCR1B |= (conf->clockSelect);
} // End-startTimer1

void stopTimer1(){
    // Deselect clock source
    TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
} // End-stopTimer1

unsigned long getTicks1(){
    return 65536 * ovfCount1 + TCNT1L + (TCNT1H << 8);
} // End-getTicks1

double getTimeUS1(Timer1* conf){
    double result = conf->prescaler * 1000000;
    result = result / F_CPU;
    return result * getTicks1();
} // End-getTime1US

double getFreq1(Timer1* conf){
    double result = F_CPU / conf->prescaler;
    return result / getTicks1();
} // End-getFreq1
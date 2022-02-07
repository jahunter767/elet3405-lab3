#include "../include/timer0.h"

volatile unsigned int ovfCount0 = 0;

ISR(TIMER0_OVF_vect){
    ovfCount0 += 1;
    PORTD ^= (1 << 7);
} // End-TIMER0_OVF ISR

void initTimer0(Timer0* conf){
    OCR0 = conf->outputCompReg;
    setTCNT0(conf);

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

    TCCR0 = (conf->forceOutputComp << FOC0) |
            (((conf->compOutputMode & 0x02) >> 1) << COM01) |
            ((conf->compOutputMode & 0x01) << COM00) |
            (((conf->waveformGenMode & 0x02) >> 1) << WGM01) |
            ((conf->waveformGenMode & 0x01) << WGM00);
} // End-initTimer0

void enableTimer0OutputCompInt(){
    TIMSK |= (1 << OCIE0);
}// End-enableTimer0OutputCompInt

void disableTimer0OutputCompInt(){
    TIMSK &= ~(1 << OCIE0);
}// End-disableTimer0OutputCompInt

void enableTimer0OvfInt(){
    TIMSK |= (1 << TOIE0);
}// End-enableTimer0OvfInt

void disableTimer0OvfInt(){
    TIMSK &= ~(1 << TOIE0);
}// End-disableTimer0OvfInt



void clearTimer0IntFlags(){
    TIFR |= (1 << OCF0) | (1 << TOV0);
} // End-clearTimer1IntFlag0

void setTCNT0(Timer0* conf){
    TCNT0 = conf->timerCounterReg;
} // End-setTCNT0

void startTimer0(Timer0* conf){
    ovfCount0 = 0;
    setTCNT0(conf);
    TCCR0 |= (conf->clockSelect);
} // End-startTimer0

void stopTimer0(){
    // Deselect clock source
    TCCR0 &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
} // End-stopTimer0

unsigned long getTicks0(){
    return 65536 * ovfCount0 + TCNT0;
} // End-getTicks0

double getTimeUS0(Timer0* conf){
    double result = conf->prescaler * 1000000;
    result = result / F_CPU;
    return result * getTicks0();
} // End-getTime0US

double getFreq0(Timer0* conf){
    double result = F_CPU / conf->prescaler;
    return result / getTicks0();
} // End-getFreq0

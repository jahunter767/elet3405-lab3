#ifndef adc_h
#define adc_h

#include "default.h"

typedef struct ADC0Struct{
    // ADMUX Bits
    unsigned char refrenceSel :2;
    unsigned char leftAdjust :1;
    unsigned char channelAndGainSel :5;

    // ADCSRA Bits
    unsigned char enable :1;
    unsigned char autoTriggerEn :1;
    unsigned char intEn :1;
    unsigned char prescalerSel :3;

    // SFIOR bits
    unsigned char autoTriggerSrc :3;
} ADC0;

void initADC(ADC0* conf);
void startConv(ADC0* conf);
int getADCVal(ADC0* conf);
int readADC(ADC0* conf);

#endif

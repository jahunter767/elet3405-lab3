#include "../include/adc.h"

void initADC(ADC0* conf){
    ADMUX &= 0x00; // Clear ADMUX
    ADMUX = (conf->refrenceSel << REFS0) |
            (conf->leftAdjust << ADLAR);

    ADCSRA &= 0x00; // Clear ADCSRA
    ADCSRA = (conf->enable << ADEN) |
             (conf->autoTriggerEn << ADATE) |
             (conf->intEn << ADIE) |
             (conf->prescalerSel);

    SFIOR &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0)); // Clear auto trigger source bits
    SFIOR |= (conf->autoTriggerSrc << ADTS0);
} // End-initADC

void startConv(ADC0* conf){
    ADMUX &= ~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));  // Clearing selected pin
    ADMUX |= (conf->channelAndGainSel); // Selecting ADC pin

    ADCSRA |= (1<<ADSC); // Starting the conversion
} // End-startConv

/* Reads the ADC value regardless of whether
   or not the conversion is complete */
int getADCVal(ADC0* conf){
    int val = 0;

    if (!conf->leftAdjust){
        val = ADCL;
        val += ((ADCH & 0x03) << 8);
    }else{
        val += ((ADCL & 0xC0) >> 5);
        val += (ADCH << 3);
    } // End-if

    return val;
} // End-getADCVal

/* Reads the ADC value without the interrupt enabled */
int readADC(ADC0* conf){
    startConv(conf);
    while (ADCSRA & (1<<ADSC)); // Wait for conversion to complete
    return getADCVal(conf);
} // End-readADC

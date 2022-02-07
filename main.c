/*
Authors: Jason Hunter, Calista Wolliston
ID Numbers: 620119029, 620121357
*/

#include "include/default.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "include/ext_int.h"
#include "include/timer1.h"
#include "include/lcd.h"
#include "include/adc.h"

// ISR Declaration
ISR(INT0_vect);
ISR(INT1_vect);
ISR(TIMER1_CAPT_vect);

// Function Declaration
void blink();
void printMode();

double readVoltage();
double readCapacitance();
double readResistance();
double readTemperature();
double readFrequency();
double readDistance();

ExtInt extInts;
Timer1 t1;
ADC0 adc;

volatile unsigned char mode = 4;
volatile unsigned char edgeCount = 0;

// ASCII: 0x43 = 'C', 0x46 = 'F'
//      : 0xE4 = mu, 0xDF = degrees
//      : 0xF4 = OMEGA
char* modeNames[6][2] = {
    {"Volt", " V"},         // 0
    {"Cap ", "\xE4\x46"},   // 1
    {"Ohm ", " \xF4"},      // 2
    {"Temp", "\xDF\x43"},   // 3
    {"Freq", "Hz"},         // 4
    {"Dist", "cm"}          // 5
};

int main (){
    /*
    // Disabling JTAG interface
    MCUCSR |= (1<<JTD);
    MCUCSR |= (1<<JTD);
    */

    // Configuring I/O
    DDRA = 0x00;
    DDRB = ((1<<PB2)|(1<<PB1)|(1<<PB0)); //0x07; 
    DDRC = 0xFF;
    DDRD = ((1<<PD7)|(1<<PD4));
    DDRD |= ((1 << PD1) | (1<<PD5));

    cli(); // Disable interrupts

    // Configuring External Interrupts
    extInts.intSenseCtrl0 = 2; // INT0 interrupts trigger on falling edge
    extInts.intSenseCtrl1 = 2; // INT1 interrupts trigger on falling edge
    initExtInt(&extInts);
    enableExtInt(0);

    // Configuring analogue comparator
    ACSR |= (1 << ACIS1) | (1 << ACIS0);

    // Configuring Timer1
    t1.waveformGenMode = 0x00; // Normal Mode
    t1.inputCapEdgeSel = 0; // Falling edge
    t1.inputCapNoiseCanceler = 1; // Enabled
    t1.clockSelect = 0x01; // No prescaler
    initTimer1(&t1);
    enableTimer1OvfInt();

    adc.refrenceSel = 1; // Reference Voltage = AVCC
    adc.leftAdjust = 0; // Right Adjusted ADC data registers
    adc.enable = 1; // Enabling ADC
    adc.prescalerSel = 6; // Pre-scaler = 64
    initADC(&adc);

    sei(); // Enabling Global Interrupts

    initLCD();

    // Variables
    double val;
    char* disp;
    unsigned char i;

    while (1){
        val = 0;
        switch (mode){
            case (0):
                printMode();
                val = readVoltage();
                break;

            case (1):
                printMode();
                val = readCapacitance();
                break;

            case (2):
                printMode();
                val = readResistance();
                break;

            case (3):
                printMode();
                val = readTemperature();
                break;

            case (4):
                printMode();
                val = readFrequency();
                break;

            case (5):
                printMode();
                val = readDistance();
                break;
        } // End-switch-case

        disp = dtostre(val, disp, 8, 0x00);
        i = 0;
        // Write out the value to the LCD
        while (disp[i] != 0){
            lcdWriteChar((0xC0 + i), disp[i]);
            i += 1;
        } // End-while

        // Fill the remaining spots with spaces
        while ((0xC0 + i) < 0xD0){
            lcdWriteChar((0xC0 + i), ' ');
            i += 1;
        } // End-while
    } // End-while

    return 1;
} // End-main

//----------------------------------------------------------------
//-------------------------ISR Definitions------------------------
//----------------------------------------------------------------

ISR(INT0_vect){
    blink();
    _delay_ms(50);

    // Stop tasks interrupted from previous mode
    // or starts task necessary for the following mode
    switch (mode){
        case (1):
            stopTimer1();

            // Deactivating analogue comparator
            DDRB |= (1 << PB2);
            SFIOR &= ~(1 << ACME);
            ACSR &= ~(1 << ACIE);

            // Discharging the capacitor
            PORTB &= ~((1 << PB2) | (1 << PB0));
            break;

        case (4):
            disableTimer1InputCapInt();
            stopTimer1();
            break;

        case (5):
            disableExtInt(1);
            break;
    } // End-switch-case

    mode = (mode + 1)%7;
} // End-INT1 ISR

ISR(INT1_vect){
    stopTimer1();
    edgeCount += 1;
    disableExtInt(1);
} // End-INT1 ISR

ISR(TIMER1_CAPT_vect){
    edgeCount += 1;
    if (edgeCount == 2){
        stopTimer1();
        disableTimer1InputCapInt();
    } // End-if
} // End-TIMER1_CAPT ISR

//----------------------------------------------------------------
//----------------------Function Definitions----------------------
//----------------------------------------------------------------

/* Blinks the led on PD7 */
void blink(){
    PORTD |= (1 << PD7);
    _delay_ms(150);
    PORTD &= ~(1 << PD7);
    _delay_ms(150);
} // End-blink

/* Writes the name of the current mode to the LCD */
void printMode(){
    lcdWrite(0x80, modeNames[mode][0]);
    lcdWrite(0x8E, modeNames[mode][1]);
} // End-printMode

double readVoltage(){
    //unsigned int Vref = 5, adcRes = 1024;
    //unsigned long R9 = 10000, R8 = 100000;
    adc.channelAndGainSel = 2;
    printMode();
    unsigned int adcVal = readADC(&adc);
    double result;

    // result = (Vref * adcVal * (R9/(R8 + R9))^-1) / 2^(adcRes)
    result = (5 * adcVal * 11) / 1024;
    return result;
} // End-readVoltage

double readCapacitance(){
    double result;

    PORTB &= ~((1 << PB2) | (1 << PB0));
    adc.channelAndGainSel = 0;
    while (readADC(&adc) != 0); // Wait for the capacitor to discharge

    // Activating analogue comparator
    DDRB &= ~(1 << PB2);
    SFIOR |= (1 << ACME);
    //ACSR |= (1 << ACIE);

    PORTD |= (1 << PD7);
    PORTB |= (1 << PB0);
    startTimer1(&t1);
    while (~ACSR & (1 << ACO)); // Wait for the capacitor to charge
    stopTimer1();
    PORTD &= ~(1 << PD7);

    //result = time_in_us / (0.693 * R);
    result = getTimeUS1(&t1) / 6930;
    return result;
} // End-readCapacitance

double readResistance(){
    adc.channelAndGainSel = 3;
    double result, Vx = readADC(&adc);
    unsigned long R1 = 10000;

    //result = (R1 * Vx) / (Vt - Vx)
    result = R1 * Vx;
    result = result / (1024 - Vx);

    return result;
} // End-readResistance

double readTemperature(){
    adc.channelAndGainSel = 1;
    double result, Vx = readADC(&adc);

    // Temperature in degrees Celcuis
    // T = (Vx * (5000 mV / 2^10)) / 10
    result = Vx * 500;
    result = result / 1024;
    return result;
} // End-readTemperature

double readFrequency(){
    double result;
    edgeCount = 0;

    /* // OLD APPROACH
    while (~TIFR & (1 << ICF1)); // Wait for next falling edge
    startTimer1(&t1); // Start Timer
    while (~TIFR & (1 << ICF1)); // Wait for next falling edge
    stopTimer1();
    */

    enableTimer1InputCapInt();
    startTimer1(&t1); // Start Timer
    while (edgeCount < 2); // Wait for one oscillation
    result = getFreq1(&t1);
    return result;
} // End-readFrequency

double readDistance(){
    double result;
    edgeCount = 0;

    enableExtInt(1);

    // Pulse the trigger pin
    PORTD |= (1 << PD4);
    _delay_us(15);
    PORTD &= ~(1 << PD4);

    startTimer1(&t1);
    while (edgeCount < 1); // Wait for next falling edge

    // distance = (34 300 cm/s x (T us / 1000000)) * 0.5
    result = 343 * getTimeUS1(&t1);
    result = result / 20000;
    return result;
} // End-readDistance

#include "../include/lcd.h"

#define LCD_EN 4
#define LCD_RW 5
#define LCD_RS 6

void pulseEnable(){
    PORTC |= (1<<LCD_EN);
    _delay_us(100);
    PORTC &= ~(1<<LCD_EN);
    _delay_ms(2);
} // End-pulseEnable

unsigned char mirrorNibble(unsigned char c){
    unsigned char i, flipped = 0x00;

    for (i = 0; i < 4; i++){
        if (c & (1 << i)){
            flipped |= (1 << (3-i));
        } //End-if
    } // End-for

    return flipped;
} // End-mirrorNibble

void send(unsigned char data){
    // Send high nibble
    PORTC &= 0xF0; // Clears data lines
    PORTC |= mirrorNibble(data >> 4);
    pulseEnable();

    // Send low nibble
    PORTC &= 0xF0; // Clears data lines
    PORTC |= mirrorNibble(data & 0x0F);
    pulseEnable();
} // End-send

void lcdCommand(unsigned char cmd){
    PORTC &= (1<<7); // Clears rs, rw, en and data bits but preserves the 8th bit
    send(cmd);
} // End-lcdCommand

void lcdWriteChar(unsigned char cursorStart, char data){
    lcdCommand(cursorStart);

    PORTC &= (1<<7); // Clears rs, rw, en and data bits but preserves the 8th bit
    PORTC |= (1<<LCD_RS);
    send(data);
    //lcdCommand(0x06); // Shift cursor to the right
} // End-lcdWriteChar

void lcdWrite(unsigned char cursorStart, char data[]){
    unsigned char i = 0;

    while (data[i] != 0){
        lcdWriteChar((cursorStart + i), data[i]);
        i += 1;
    } // End-while
} // End-lcdWrite

//Initializes the LCD in 4 bit mode
void initLCD(){
    DDRC |= 0x7F;
    _delay_ms(10);
    lcdCommand(0x33);
    lcdCommand(0x32);
    lcdCommand(0x28);
    lcdCommand(0x0C);
    lcdCommand(0x01);
    _delay_ms(20);
} // End-initLCD

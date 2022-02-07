#ifndef lcd_h
#define lcd_h

#include "default.h"
#include <util/delay.h>

void pulseEnable();
unsigned char mirrorNibble(unsigned char c);
void send(unsigned char data);

void lcdCommand(unsigned char cmd);
void lcdWriteChar(unsigned char cursorStart, char data);
void lcdWrite(unsigned char cursorStart, char data[]);

void initLCD();

#endif

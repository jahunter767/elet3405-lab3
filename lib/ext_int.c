#include "../include/ext_int.h"

void initExtInt(ExtInt* conf){
    // Clears the relevant bits
    MCUCR &= ~(((1 << ISC01) | (1 << ISC00)) |
              ((1 << ISC11) | (1 << ISC10)));

    // Sets relevant bits
    MCUCR |= ((((conf->intSenseCtrl1 & 0x02) >> 1) << ISC11) |
              ((conf->intSenseCtrl1 & 0x01) << ISC10) |
              (((conf->intSenseCtrl0 & 0x02) >> 1) << ISC01) |
              ((conf->intSenseCtrl0 & 0x01) << ISC00));

    // Clears the relevant bits
    MCUCSR &= ~(1 << ISC2);

    // Sets the relevant bits
    MCUCSR |= (conf->intSenseCtrl2 << ISC2);
} // End-initExtInt

/* Enables the specified external interrupts.
   255 enables all external interrupts. */
void enableExtInt(unsigned char i){
    // Sets the relevant bits
    switch (i){
        case (0):
            GICR |= (1 << INT0);
            break;
        case (1):
            GICR |= (1 << INT1);
            break;
        case (2):
            GICR |= (1 << INT2);
            break;
        case (255):
            GICR |= ((1 << INT2) | (1 << INT1) | (1 << INT0));
            break;
    } // End-switch-case
} // End-enableExtInt

/* Disables the specified external interrupts.
   255 disables all external interrupts. */
void disableExtInt(unsigned char i){
    // Clears the relevant bits
    switch (i){
        case (0):
            GICR &= ~(1 << INT0);
            break;
        case (1):
            GICR &= ~(1 << INT1);
            break;
        case (2):
            GICR &= ~(1 << INT2);
            break;
        case (255):
            GICR &= ~((1 << INT2) | (1 << INT1) | (1 << INT0));
            break;
    } // End-switch-case
} // End-disableExtInt

/* Clears the specified external interrupt flags.
   255 clears all external interrupt flags. */
void clearExtIntFlags(unsigned char f){
    // Clears the relevant bits
    switch (f){
        case (0):
            GIFR |= (1 << INTF0);
            break;
        case (1):
            GIFR |= (1 << INTF1);
            break;
        case (2):
            GIFR |= (1 << INTF2);
            break;
        case (255):
            GIFR |= ((1 << INTF2) | (1 << INTF1) | (1 << INTF0));
            break;
    } // End-switch-case
} // End-clearExtIntFlags

/*
 * File:   sev_seg.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 4:06 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

int dig0, dig1, dig2, dig3; //Initialize variables to store individual 7-seg numbers

const unsigned char segTable[11] = {
    0b11000000, 0b11111001,
    0b10100100, 0b10110000,
    0b10011001, 0b10010010,
    0b10000010, 0b11111000,
    0b10000000, 0b10010000, 0b11111111
}; //Array of bytes corresponding to digits 0-9 & blank

void sevSegNum(void) {

    //Formulas to derive each digit of water level
    dig0 = waterLvl % 10;
    dig1 = (waterLvl % 100) / 10;
    dig2 = (waterLvl % 1000) / 100;
    dig3 = waterLvl / 1000;

    PORTAbits.RA5 = 0; //1st panel from right
    PORTAbits.RA4 = 0;
    PORTC = segTable[dig0];
    __delay_ms(5);

    PORTAbits.RA5 = 0; //2nd panel from right
    PORTAbits.RA4 = 1;
    if (waterLvl < 10) //Turn off panel if not req
        PORTC = segTable[10];
    else
        PORTC = segTable[dig1];
    __delay_ms(5);

    PORTAbits.RA5 = 1; //3rd panel from right
    PORTAbits.RA4 = 0;
    if (waterLvl < 100) //Turn off panel if not req
        PORTC = segTable[10];
    else
        PORTC = segTable[dig2];
    __delay_ms(5);

    PORTAbits.RA5 = 1; //4th panel from right
    PORTAbits.RA4 = 1;
    if (waterLvl < 1000) //Turn off panel if not req
        PORTC = segTable[10];
    else
        PORTC = segTable[dig3];
    __delay_ms(5);
}

void sevOff(void) {
    //Turn off 7-segment display.
    PORTAbits.RA5 = 1;
    PORTAbits.RA4 = 1;
    PORTC = segTable[10];

    PORTAbits.RA5 = 1;
    PORTAbits.RA4 = 0;
    PORTC = segTable[10];

    PORTAbits.RA5 = 0;
    PORTAbits.RA4 = 1;
    PORTC = segTable[10];

    PORTAbits.RA5 = 0;
    PORTAbits.RA4 = 0;
    PORTC = segTable[10];

}
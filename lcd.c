/*
 * File:   lcd.c
 * Author: HUMANOID
 *
 * Created on 13 February, 2024, 7:33 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

#define enableLCD PORTEbits.RE0
#define regSelLCD PORTEbits.RE1

void sendLCD(char data, unsigned char regSel) {
    regSelLCD = regSel; //Set to logic 0 to send control instructions or 1 to send data word
    PORTD = data; //Send upper nibble
    enableLCD = 1;
    __delay_ms(1);
    enableLCD = 0;
    __delay_ms(1); //Trigger falling edge pulse
    data <<= 4;
    PORTD = data; //Send lower nibble
    enableLCD = 1;
    __delay_ms(1);
    enableLCD = 0;
    __delay_ms(1); //Trigger falling edge pulse
}

void startLCD(void) {
    __delay_ms(15);
    sendLCD(0b00000011, 0);
    __delay_ms(5);
    sendLCD(0b00000010, 0);

    sendLCD(0b00101000, 0); //Function Set (4-bit DL, 2 lines, 5*7 dots)
    sendLCD(0b00001100, 0); //Disp on, cursor off, blink off
    sendLCD(0b00000110, 0); //Entry Mode (Increment pos,no shift)
    sendLCD(0b00000001, 0); //Clear display & address counter
}

void msgLCD(char msg[]) { //Send message to be displayed one character at a time
    int i;
    for (i = 0; msg[i] != 0; i++)
        sendLCD(msg[i], 1);
}

void LCD_setPos(unsigned char row, unsigned char col) {
    unsigned char position; // Control word to be sent

    if (row == 1) // If row is 1, subtract 1 from col
        position = col - 1;
    else // If row is 2, add 0x40 to position & subtract 1 from col
        position = 0x40 + col - 1;

    sendLCD(0x80 + position, 0); // Add 0x80 to position and send data
}

void LCD_UpdatePower(void) {
    LCD_setPos(2, 11);

    char powerLvl_str[4] = {};
    sprintf(powerLvl_str, "%d", powerLvl); //Convert int to string

    msgLCD(powerLvl_str); //Disply on LCD
}

void clearTopLCD(void) {
    LCD_setPos(1, 1);
    msgLCD("                "); //Clear message on line 1
}

void clearBtmLCD(void) {
    LCD_setPos(2, 1);
    msgLCD("                "); //Clear message on line 2
}

/*
 * File:   led.c
 * Author: HUMANOID
 *
 * Created on 15 February, 2024, 3:30 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

//Define the bit corresponding to each LED to avoid interfering with the other Port A bits
#define LED1 PORTAbits.RA3
#define LED2 PORTAbits.RA2
#define LED3 PORTAbits.RA1

int openLED_status = 0; //Disable both LED sequences to start
int closeLED_status = 0;

void offLED(void) { //Turn off all LEDs
    LED1 = 0, LED2 = 0, LED3 = 0;
}

void emergency_LED(void) { //Turn on all LEDs to indicate emergency
    LED1 = 1, LED2 = 1, LED3 = 1;
}

void openLED(void) { //LED blinking pattern when gate is opening
    T5CONbits.ON = 1; //Enable Timer5
    openLED_status = 1; //Enable openLed

    LED1 = 1, LED2 = 0, LED3 = 1;
}

void closeLED(void) { //LED blinking pattern when gate is closing
    openLED_status = 0; //Disable openLED
    closeLED_status = 1; //Enable closeLED

    LED1 = 1, LED2 = 0, LED3 = 1;
}

void powerONLED(int time) { //LED blinking pattern all other times when power production is on
    LED1 = 1, LED2 = 0, LED3 = 0;
    custom_delayLED(time); ////Function for custom delay time
    LED1 = 0, LED2 = 1, LED3 = 0;
    custom_delayLED(time);
    LED1 = 0, LED2 = 0, LED3 = 1;
    custom_delayLED(time);
    LED1 = 0, LED2 = 1, LED3 = 0;
    custom_delayLED(time);
}

void custom_delayLED(int rep) { //Function for custom delay time
    int i;
    for (i = 0; i < rep; i++) {
        __delay_ms(1);
    }
}



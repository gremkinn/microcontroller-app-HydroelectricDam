/*
 * File:   main.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 3:30 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

int waterLvl = 0; //Store ADC conversion from potentiometer to simulate water level
int powerLvl = 0; //Store generated power

void main(void) {
    initSysPins(); //Configure pins
    startLCD(); //Initialize LCD

    INTCONbits.GIE = 0; //Disable global interrupts
    INTCONbits.PEIE = 0; //Disable peripheral interrupts

    initExtint_RB1(); //Initialise external interrupt
    initIOC_RB4(); //Initalise interrupt on change
    initADC(); //Initialize ADC

    //Initialize timers
    initTimer0_ADC(); //T0 to use with ADC
    initTimer1_LCD(); //T1 with LCD
    initTimer2_sevseg(); //T2 for 7-seg
    initTimer3_gateClose(); //T3 for gateClose
    initTimer4_drainWater(); //T4 to decrement waterLvl
    initTimer5_LED(); //T5 for LEDs

    INTCONbits.GIE = 1; //Enable global interrupts
    INTCONbits.PEIE = 1; //Enable peripheral interrupts

    offLED(); //All LEDS off to start

    while (1) {
        norm_protocol();
        if (waterLvl >= 3500)
            openGate();
    }


}

void initSysPins(void) {
    ANSELA = 0b00000001;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    TRISA = 0b11000001;
    TRISB = 0b11110111;
    TRISC = 0;
    TRISD = 0x0F;
    TRISE = 0b11111100;
}

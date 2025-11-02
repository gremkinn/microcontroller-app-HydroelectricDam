/*
 * File:   prog_functions.c
 * Author: HUMANOID
 *
 * Created on 15 February, 2024, 4:19 PM
 */
#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

extern int gate_count; //Count cycles of timer
extern int openLED_status; //Toggle openLED
extern int closeLED_status; //Toggle closeLED

void norm_protocol(void) {
    ////Display power gen speed
    LCD_setPos(2, 5);

    if (PORTBbits.RB4 == 1) {
        if (PORTBbits.RB2 == 1) //If RB2 is up, 1kW produced every 1 seconds
            msgLCD("+");
        else if (PORTBbits.RB2 == 0) //If RB2 is up, 1kW produced every 2 seconds
            msgLCD("-");
    }

    clearTopLCD();
    LCD_setPos(1, 3);
    msgLCD("[ HydroDam ]");

    while (1) {
        if (waterLvl >= 3500)
            break;
        //Match LED change time to power gen speed
        if (PORTBbits.RB4 == 1) {
            if (PORTBbits.RB2 == 0)
                powerONLED(1750); //LEDs change slower
            else if (PORTBbits.RB2 == 1)
                powerONLED(850); //LEDs change faster
        } else if (PORTBbits.RB4 == 0) {
            offLED();
        }
    }
}

void openGate(void) {

    T0CON0bits.T0EN = 0; //Disable T0 that works with ADC
    PIE0bits.INTE = 1; //Enable external interrupt for emergency control

    ADCON0bits.ADON = 0; //Disable ADC
    LCD_setPos(1, 3);
    msgLCD("GATE OPENING");

    openLED(); //Change LED pattern on interrupt
    T4CONbits.ON = 1; //Enable T4 that works to decrement waterLvl

    while (waterLvl > 3000) {
        openBuzzer();
    }

    openLED_status = 0; //Disable openLED
    closeGate();
}

void closeGate(void) {
    LCD_setPos(1, 3);
    msgLCD("GATE CLOSING"); //Disp msg
    T3CONbits.ON = 1; //Enable T3 to fix the gate closing to 5 sec
    closeLED();
    while (gate_count != 10) { //Track no. of 0.5sec cycles T3 has completed
        closeBuzzer();
    }
    T5CONbits.ON = 0; //Disable T5 that works with LED
    offLED();
    T4CONbits.ON = 0; //Disable T4
    ADCON0bits.ADON = 1; //Enable ADC
    T0CON0bits.T0EN = 1; //Enable T0 that works with ADC
    closeLED_status = 0; //Disable closeLed
    PIE0bits.INTE = 0; //Disable external interrupt for emergency control
}

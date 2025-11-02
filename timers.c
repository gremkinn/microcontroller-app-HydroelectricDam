    /*
 * File:   timers.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 3:37 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

void initTimer0_ADC(void) { //Timer duration = 3 sec
    T0CON0 = 0b10010000; //Enable Timer0, 16-bit mode, Postscaler = 1
    T0CON1 = 0b01000110; //FOSC/4, Sync to clock, Prescaler = 64
    TMR0H = 0x48;
    TMR0L = 0xE4; //Set registers to 18660 or 0x48E4
    PIR0bits.TMR0IF = 0; //Clear T0 Interrupt Flag
    PIE0bits.TMR0IE = 1; //Enable Timer0 Interrupt
}

void initTimer1_LCD(void) { //Timer Duration = 0.5 sec
    T1CLK = 0b00000001; //Set timer1 clock to Fosc/4
    T1CON = 0b00110011; //Enable Timer1, Prescaler = 8
    TMR1H = 0x24;
    TMR1L = 0x46; //Set registers to 9286 or 0x2446, Interrupt every 0.45 sec
    PIR4bits.TMR1IF = 0; //Clear T1 Interrupt Flag
    PIE4bits.TMR1IE = 1; //Enable Timer1 Interrupt

    if (PORTBbits.RB4 == 1) { //Check if power gen is on/off
        LCD_setPos(2, 1);
        msgLCD("Spd:  Pwr:    kW");
    } else if (PORTBbits.RB4 == 0) {
        T1CONbits.ON = 0;
        LCD_setPos(2, 2);
        msgLCD("Power Gen: OFF");
    }
}

void initTimer2_sevseg(void) { //Timer Duration = 8 ms
    T2CLKCON = 0b00000001; //Set timer2 clock to Fosc/4
    T2CON = 0b11010000; //Timer on,Npre = 32; Npost = 1
    TMR2 = 249;
    PIR4bits.TMR2IF = 0; //Clear T2 Interrupt Flag
    PIE4bits.TMR2IE = 1; //Enable T2
}

void initTimer3_gateClose(void) { //Set to 500ms
    T3CLK = 0b00000001; //Set timer1 clock to FOSC/4
    T3CON = 0b00110010; //Nrpe = 8, Disable Timer
    TMR3H = 0x0B;
    TMR3L = 0xDC; //Set registers to 3036 or 0x0BDC
    PIR4bits.TMR3IF = 0; //Clear T3 Interrupt Flag
    PIE4bits.TMR3IE = 1; //Enable Timer3 Interrupt   
}

void initTimer4_drainWater(void) { //Timer duration approx 50ms
    T4CLKCON = 0b00000001; //Set timer4 clock to Fosc/4
    T4CON = 0b01001011; //Timer off,Npre = 16,Npost = 12
    TMR4 = 255;
    PIR4bits.TMR4IF = 0; //Clear interrupt flag
    PIE4bits.TMR4IE = 1; //Enable interrupt
}

void initTimer5_LED(void) { //Set to 500ms
    T5CLK = 0b00000001; //Set timer5 clock to FOSC/4
    T5CON = 0b00110010; //Disable Timer1, Prescaler = 8
    TMR5H = 0x0B;
    TMR5L = 0xDC; //Set registers to 3036 or 0x0BDC
    PIR4bits.TMR5IF = 0; //Clear T5 Interrupt Flag
    PIE4bits.TMR5IE = 1; //Enable Timer5 Interrupt   
}
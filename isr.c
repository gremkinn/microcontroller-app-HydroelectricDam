/*
 * File:   isr.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 4:03 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

//Define the bit corresponding to each LED to avoid interfering with the other Port A bits
#define LED1 PORTAbits.RA3
#define LED2 PORTAbits.RA2
#define LED3 PORTAbits.RA1

extern int openLED_status; //Toggle closeLED
extern int closeLED_status; //Toggle openLed
extern int dig0, dig1, dig2, dig3; //Initialize variables to store individual 7-seg numbers

//Variables to count timer cycles
int power_count = 0;
int gate_count = 0;
int sev_sg_count = 0;

void initExtint_RB1(void) { //Initialize external interrupt
    PIR0bits.INTF = 0; //Clear extint flag
    INTPPS = 0x09; //Map extint to RB1
    INTCONbits.INTEDG = 1; //Detect rising edge
    PIE0bits.INTE = 0; //Disable external interrupt until req for emergency control
}

void initIOC_RB4(void) { //Init IOC to RB4
    PIR0bits.IOCIF = 0; //Clear IOC interrupt flag
    IOCBFbits.IOCBF4 = 0; //Clear RB4 Interrupt flag
    PIE0bits.IOCIE = 1; //Enable interrupt
    IOCBNbits.IOCBN4 = 1; //Detect falling edge
    IOCBPbits.IOCBP4 = 1; //Detect rising edge
}

void __interrupt() isr(void) {

    //ISR for Extint
    if (PIR0bits.INTF == 1 && INTPPS == 0x09) { //Check for RB1 flag
        PIR0bits.INTF = 0; //Clear interrupt flag
        INTPPS = 0x08; //Re-route extint to RB0 for resume button
        sevOff(); //Turn off SevSeg panels
        emergency_LED();
        LCD_setPos(1, 2);
        msgLCD("EMERGENCY STOP");

        while (1) {
            if (PIR0bits.INTF == 1) { //Check for RB0 flag
                PIR0bits.INTF = 0; //Clear interrupt flag
                INTPPS = 0x09; //Re-route extint to RB1 for emergency control
                resumeBuzzer();
                clearTopLCD();

                LCD_setPos(1, 3);
                if (waterLvl < 3000)
                    msgLCD("GATE CLOSING");
                else
                    msgLCD("GATE OPENING");

                break;
            }
            emergencyBuzzer();
        }
    }

    //ISR for IOC
    if (IOCBFbits.IOCBF4 == 1) { //Interrupt for RB4 as electricity production toggle
        PIR0bits.IOCIF = 0; //Clear flag
        IOCBFbits.IOCBF4 = 0; //Clear flag

        clearBtmLCD();

        if (PORTBbits.RB4 == 1) {
            T1CONbits.ON = 1; //Enable Timer1 for power gen
            LCD_setPos(2, 1);
            msgLCD("Spd:  Pwr:    kW");
        } else if (PORTBbits.RB4 == 0) {
            LCD_setPos(2, 2);
            T1CONbits.ON = 0; //Disable Timer1
            msgLCD("Power Gen: OFF");
        }
    }

    //ISR for ADC + Timer0 Interrupt for water level
    if (PIR0bits.TMR0IF == 1) { //Check if T0 flag is raised
        PIR0bits.TMR0IF = 0; //Clear T0 Interrupt flag
        TMR0H = 0x48;
        TMR0L = 0xE5; //Reset Timer0 registers
        get_waterLvl(); //Get water level from ADC
    }

    //ISR with Timer1 for LCD - generated power
    if (PIR4bits.TMR1IF == 1) {

        power_count++; //Increment power count to count cycles of timer that have passed

        if (PORTBbits.RB2 == 1 && power_count == 2) { //If RB2 is up, 1kW produced every 1 seconds
            LCD_setPos(2, 5);
            msgLCD("+"); //Display power generation speed level
            powerLvl++; //Increment generated power
            power_count = 0; //Reset cycle counter
            LCD_UpdatePower(); //DIsp on LCD

        } else if (PORTBbits.RB2 == 0 && power_count == 4) { //If RB2 is up, 1kW produced every 2 seconds
            LCD_setPos(2, 5);
            msgLCD("-"); //Display power generation speed level
            powerLvl++; //Increment generated power
            power_count = 0; //Reset cycle counter
            LCD_UpdatePower(); //Disp on LCD

        } else if (power_count > 4) {
            power_count = 0; //Reset cycle counter
        }

        PIR4bits.TMR1IF = 0; //Clear interrupt flag
        TMR1H = 0x24;
        TMR1L = 0x46; //Set registers to 9286 or 0x2446, Interrupt every 0.45 sec
    }

    if (PIR4bits.TMR2IF == 1) {

        if (sev_sg_count == 0) {
            dig0 = waterLvl % 10; //Derive digit
            PORTAbits.RA5 = 0; //1st panel from right
            PORTAbits.RA4 = 0;
            PORTC = segTable[dig0];
            sev_sg_count++;

        } else if (sev_sg_count == 1) {
            dig1 = (waterLvl % 100) / 10; //Derive digit
            PORTAbits.RA5 = 0; //2nd panel from right
            PORTAbits.RA4 = 1;
            if (waterLvl < 10) //Turn off panel if not req
                PORTC = segTable[10];
            else
                PORTC = segTable[dig1];

            sev_sg_count++;
        } else if (sev_sg_count == 2) {
            dig2 = (waterLvl % 1000) / 100; //Derive digit
            PORTAbits.RA5 = 1; //3rd panel from right
            PORTAbits.RA4 = 0;
            if (waterLvl < 100) //Turn off panel if not req
                PORTC = segTable[10];
            else
                PORTC = segTable[dig2];

            sev_sg_count++;
        } else if (sev_sg_count == 3) {
            dig3 = waterLvl / 1000; //Derive digit
            PORTAbits.RA5 = 1; //4th panel from right
            PORTAbits.RA4 = 1;
            if (waterLvl < 1000) //Turn off panel if not req
                PORTC = segTable[10];
            else
                PORTC = segTable[dig3];
            sev_sg_count = 0;
        }
        PIR4bits.TMR2IF = 0; //Clear flag
    }

    //ISR with Timer3 for gate closing to last 5 seconds
    if (PIR4bits.TMR3IF == 1) {
        PIR4bits.TMR3IF = 0; //Clear flag
        TMR5H = 0x0B;
        TMR5L = 0xDC; //Reset registers to 3036 or 0x0BDC
        gate_count++; //Increment cycle counter
        if (gate_count == 10) {
            T3CONbits.ON = 0; //Disable T3
            gate_count = 0; //Reset cycle counter
        }
    }

    //ISR with Timer4 for waterLvl decrement
    if (PIR4bits.TMR4IF == 1) {
        PIR4bits.TMR4IF = 0; //Clear flag
        waterLvl--; //Decrement waterLvl for draining
    }

    //ISR with Timer5 for LED control
    if (PIR4bits.TMR5IF == 1) {
        PIR4bits.TMR5IF = 0; //Clear flag

        if (openLED_status == 1) {
            TMR3H = 0x0B;
            TMR3L = 0xDC; //Reset registers to 3036 or 0x0BDC
            LED1 = ~LED1, LED2 = 0;
            LED3 = ~LED3;
        } else if (closeLED_status == 1) {
            TMR3H = 0x0B;
            TMR3L = 0xDC; //Reset registers to 3036 or 0x0BDC
            LED1 = ~LED1, LED2 = ~LED2, LED3 = ~LED3;
        }
    }
}
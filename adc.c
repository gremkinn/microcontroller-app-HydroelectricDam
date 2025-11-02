/*
 * File:   sev_seg.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 3:13 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

void initADC(void) {
    ADREF = 0; //ADC reference selection, set VREF+ and VREF- to VSS and VDD
    ADCLK = 0b00000011; //ADC clock selection, set TAD = 2us
    ADACQ = 0; //Manually insert acquisition time
    ADCON0 = 0b10000101; //Enable ADC, single conversion, FOSC clock, right justify, enable conversion
    ADPCH = 0; //RA0 channel to collect input from potentiometer
}

void get_waterLvl(void) {
    __delay_us(2); //Insert acquisition time
    ADCON0bits.ADGO = 1; //Start conversion 
    while (ADCON0bits.ADGO == 1); //Wait for completion
    waterLvl = (ADRESH * 256 + ADRESL)*4; //Store result in variable & multiply to simulate water level
}

/*
 * File:   buzzer.c
 * Author: HUMANOID
 *
 * Created on 11 February, 2024, 5:46 PM
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "global.h"

//Function declarations of different musical notes, that each have a unique frequency.
void A_hi(float);
void B_low(float);
void B_hi(float);
void C_low(float);
void C_hi(float);
void D(float);
void E(float);
void G(float);
void F(float);
void F_sharp(float);
void space(float); //No sound

void A_hi(float sec) {
    int x;
    int loopNum = sec / 0.0021;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1050); //466Hz
        PORTBbits.RB3 = 0;
        _delay(1050);
    }
}

void B_low(float sec) {
    int x;
    int loopNum = sec / 0.0041;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(2050); //246Hz
        PORTBbits.RB3 = 0;
        _delay(2050);
    }
}

void B_hi(float sec) {
    int x;
    int loopNum = sec / 0.002;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1000); //493Hz
        PORTBbits.RB3 = 0;
        _delay(1000);
    }
}

void C_low(float sec) {
    int x;
    int loopNum = sec / 0.0077;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(3850); //130Hz
        PORTBbits.RB3 = 0;
        _delay(3850);
    }
}

void C_hi(float sec) {
    int x;
    int loopNum = sec / 0.0039;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1950); //261Hz
        PORTBbits.RB3 = 0;
        _delay(1950);
    }
}

void D(float sec) {
    int x;
    int loopNum = sec / 0.0034;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1700); //293Hz
        PORTBbits.RB3 = 0;
        _delay(1700);
    }
}

void E(float sec) {
    int x;
    int loopNum = sec / 0.003;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1500); //329Hz
        PORTBbits.RB3 = 0;
        _delay(1500);
    }
}

void G(float sec) {
    int x;
    int loopNum = sec / 0.0026;

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1300); //392Hz
        PORTBbits.RB3 = 0;
        _delay(1300);
    }
}

void F(float sec) {
    int x;
    int loopNum = sec / 0.0029; //Formula to derive no. of loops based on play time required

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1450); //349Hz
        PORTBbits.RB3 = 0;
        _delay(1450);
    }
}

void F_sharp(float sec) {
    int x;
    int loopNum = sec / 0.0027; //Formula to derive no. of loops based on play time required

    for (x = 0; x < loopNum; x++) {
        PORTBbits.RB3 = 1;
        _delay(1350); //233Hz
        PORTBbits.RB3 = 0;
        _delay(1350);
    }
}

void space(float sec) { //No sound
    int x;
    int loopNum = sec * 1000000;
    PORTBbits.RB3 = 0;
    for (x = 0; x < loopNum; x++)
        _delay(1);
}

void closeBuzzer(void) { //Harry Potter Main theme for gate closing tone
    B_hi(0.2);
    E(0.37);
    G(0.112);
    F_sharp(0.25);
    E(0.5);
    B_hi(0.25);
    A_hi(0.75);
    F_sharp(0.75);
    E(0.25);
    G(0.15);
    F_sharp(0.25);
    E(0.37);
    F_sharp(0.25);
    B_low(0.74);
}

void openBuzzer(void) {//Pirates of the Caribbean main theme for gate opening
    A_hi(0.225);
    C_hi(0.1);
    D(0.3);
    space(0.2);
    D(0.225);
    space(0.2);
    D(0.127);
    E(0.225);
    F(0.3);
    space(0.2);
    F(0.225);
    space(0.2);
    F(0.15);
    G(0.225);
    E(0.225);
    space(0.2);
    E(0.3);
    D(0.15);
    C_low(0.1);
    space(0.05);
    C_low(0.1);
    D(0.05);
}

void emergencyBuzzer(void) { //Game of Thrones main theme for emergency procedure
    F_sharp(0.4);
    space(0.4);
}

void resumeBuzzer(void) { //Simple short tone
    B_hi(0.3);
    space(0.1);
    F_sharp(0.4);
    space(0.1);
    B_hi(0.3);
}





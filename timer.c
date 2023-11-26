/*
 * File:   timer.c
 * Author: weiye
 *
 * Created on June 29, 2023, 5:38 PM
 */


#include <xc.h>
#include "config.h"

unsigned int tmr_TimeSecs,tmr_TimeMins,tmr_TimeHours= 0;

void initSysTimer0(void) { //delay of 1 sec
    INTCONbits.GIE = 0;
    T0CON0 = 0b10010000;
    T0CON1 = 0b01000100;
    TMR0H = 0x0B; 
    TMR0L = 0xDC;
    PIR0bits.TMR0IF = 0;
    PIE0bits.TMR0IE = 1;
    INTCONbits.GIE = 1;
}

unsigned int tmr_GetTimeSecs(void){
    return tmr_TimeSecs;
}

unsigned int tmr_GetTimeMins(void){
    return tmr_TimeMins;
}

unsigned int tmr_GetTimeHours(void){
    return tmr_TimeHours;
}


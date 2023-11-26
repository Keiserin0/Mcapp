/*
 * File:   isr.c
 * Author: weiye
 *
 * Created on July 2, 2023, 1:53 PM
 */


#include <xc.h>
#include "config.h"

void lcdWriteDspData(char x);
void lcdCtrl_SetPos(unsigned char row, unsigned char col);

void dsp_checkSystem(void);
void intDspTask_onLCD(void);

unsigned int tmr_TimeSecs,tmr_TimeMins,tmr_TimeHours = 0;
unsigned int oxygenLevel = 79;

void __interrupt() isr(void) {
    if (PIR0bits.INTF){
        while(PORTBbits.RB5==1){
            intDspTask_onLCD();
            PIR0bits.INTF = 0;
        }
    }
    if (PIR0bits.TMR0IF == 1) { //Timer count
        TMR0H = 0x0B; 
        TMR0L = 0xDC;
        tmr_TimeSecs++;
        if (tmr_TimeSecs>59){
            tmr_TimeSecs=0;
            tmr_TimeMins++;
            if (tmr_TimeMins>59){
                tmr_TimeMins = 0;
                tmr_TimeHours++;
                if (tmr_TimeHours>23){
                    tmr_TimeHours=0;
                }
            }
        }
        PIR0bits.TMR0IF = 0;
    }
    if (IOCBFbits.IOCBF0 == 1){
        oxygenLevel++;
        if(oxygenLevel>99){
            oxygenLevel=99;
        }
        IOCBFbits.IOCBF0 = 0;
    }
    if (IOCBFbits.IOCBF1 == 1){
        oxygenLevel--;
        if (oxygenLevel<0){
            oxygenLevel=0;
        }
        IOCBFbits.IOCBF1 = 0;
    }
}


    

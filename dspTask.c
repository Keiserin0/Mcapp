/*
 * File:   dspTask.c
 * Author: weiye
 *
 * Created on July 3, 2023, 10:39 PM
 */


#include <xc.h>
#include "config.h"

#define EXTINT PORTBbits.RB5

void lcdWriteDspData(char x);
void lcdCtrl_ClearDisplay();
void lcdCtrl_SetPos(unsigned char row, unsigned char col);
void seg_DispAll(unsigned int count);

unsigned int curr_secs, curr_mins, curr_hours;

unsigned int tmr_GetTimeSecs(void);
unsigned int tmr_GetTimeMins(void);
unsigned int tmr_GetTimeHours(void);
unsigned int adc_GetConversion_Temp(void);
unsigned int adc_GetConversion_pH(void);

unsigned int getOxygenLvl(void);
unsigned char dig0, dig1;
unsigned int phLvl, temperature;

char clearMsg[] = "        ", checkingMsg[] = "Checking", waterPump[] = "Pumping";
char WARNINGHIGH[] = " HIGH ", WARNINGLOW[] = " LOW  ", NORMAL[] = "NORMAL";
char phLvlMsg[] = "pH level:";
char emergencyMsg[] = "EMERGENCY!      ";
char statusMsg[] = "NET RELEASED   ";

#define BUZZER      PORTAbits.RA2

void dspTask_OnLCD_Timer(void) {
    unsigned char sec0, sec1, min0, min1, hour0, hour1;

    curr_secs = tmr_GetTimeSecs();
    curr_mins = tmr_GetTimeMins();
    curr_hours = tmr_GetTimeHours();

    sec0 = curr_secs % 10;
    sec1 = (curr_secs / 10) % 10;
    lcdCtrl_SetPos(1, 14);
    lcdWriteDspData(sec1 + 0x30);
    lcdWriteDspData(sec0 + 0x30);

    min0 = curr_mins % 10;
    min1 = (curr_mins / 10) % 10;
    lcdCtrl_SetPos(1, 11);
    lcdWriteDspData(min1 + 0x30);
    lcdWriteDspData(min0 + 0x30);

    hour0 = curr_hours % 10;
    hour1 = (curr_hours / 10) % 10;
    lcdCtrl_SetPos(1, 8);
    lcdWriteDspData(hour1 + 0x30);
    lcdWriteDspData(hour0 + 0x30);
}

void dspTask_OnLCD_Temp(void) {
    temperature = adc_GetConversion_Temp();

    dig0 = temperature % 10;
    dig1 = (temperature / 10) % 10;

    lcdCtrl_SetPos(2, 13);
    lcdWriteDspData(dig1 + 0x30);
    lcdWriteDspData(dig0 + 0x30);
}

void dspTask_OnSevSeg_OxygenLevel(void) {
    unsigned int count;

    count = getOxygenLvl();

    seg_DispAll(count);
}

void dspTask_onLCD_pHLevel(void) {
    unsigned char dig0, dig1;

    phLvl = adc_GetConversion_pH();

    dig0 = phLvl % 10;
    dig1 = (phLvl / 10) % 10;

    lcdCtrl_SetPos(2, 10);
    lcdWriteDspData(dig1 + 0x30);
    lcdWriteDspData(dig0 + 0x30);
}

void intDspTask_onLCD(void){
    PORTBbits.RB3 = 1;
    lcdCtrl_SetPos(1, 1);
    for(unsigned int i=0; emergencyMsg[i]!=0; i++){
        lcdWriteDspData(emergencyMsg[i]);
    }
    lcdCtrl_SetPos(2, 1);
    for(unsigned int i=0; statusMsg[i]!=0; i++){
        lcdWriteDspData(statusMsg[i]);
    } 
    for(int i=0; i<2; i++){
        for(int count=0; count<1000; count++){
            BUZZER = 1;
            __delay_us(500);
            BUZZER = 0;
            __delay_us(500);
        }
    }
    lcdCtrl_ClearDisplay();
    PORTBbits.RB3 = 0;
}

unsigned int getPHLvl(void) {
    return phLvl;
}

unsigned int getTemp(void) {
    return temperature;
}
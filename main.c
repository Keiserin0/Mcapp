/*
 * File:   main.c
 * Author: weiye
 *
 * Created on June 29, 2023, 5:08 PM
 */


#include <xc.h>
#include "config.h"

#define BUZZER PORTAbits.RA2
#define MOTOR PORTAbits.RA3
#define RED1 PORTBbits.RB3
#define YELLOW1 PORTBbits.RB4
#define RED2 PORTDbits.RD2
#define YELLOW2 PORTDbits.RD3

void initSysPins(void);
void initSysTimer0(void);
void initLCD(void);
void initADC(void);
void initSysExtInt(void);
void lcdWriteDspData(char x);
void lcdCtrl_SetPos(char row, char col);
void lcdCtrl_ClearDisplay(void);
void dspTask_OnLCD_Timer(void);
void dspTask_OnLCD_Temp(void);
void dspTask_OnSevSeg_OxygenLevel(void);
void dspTask_onLCD_pHLevel(void);
unsigned int getOxygen_Dig1Lvl(void);
unsigned int getPHLvl(void);
unsigned int getTemp(void);
void checkInputMenu(void);
void pumpOxygenLED(void);
void pumpCO2LED(void);

unsigned int curr_secs,curr_mins,curr_hours, phLvl;
unsigned char dig0, dig1;
extern int selection;
unsigned int oxygen_dig1;
unsigned int temperature;

char clearMsg[]="        ",checkingMsg[]="Checking",waterPump[]="Pumping";
//char WARNINGHIGH[] = " HIGH ",WARNINGLOW[] = " LOW  ",NORMAL[] = "NORMAL";


void main(void) {
    char timer[]="Timer:";
    char colon[]=":";
    
    char temp[] = "Temperature:";
    char percentage[] = "C";
    
    char phLvlMsg[] = "pH Level:";
    initSysPins();
    initSysExtInt();
    initSysTimer0();
    initADC();
    initLCD();

    unsigned int i;
    while (1) {
        lcdCtrl_SetPos(1, 1); //Timer message
        for (i = 0; timer[i] !=0; i++) {
            lcdWriteDspData(timer[i]);
        }

        lcdCtrl_SetPos(1, 10); //colon
        for (i = 0; colon[i] !=0; i++) {
            lcdWriteDspData(colon[i]);
        }

        lcdCtrl_SetPos(1, 13); //colon
        for (i = 0; colon[i] !=0; i++) {
            lcdWriteDspData(colon[i]);
        }
        dspTask_OnLCD_Timer();
        dspTask_OnSevSeg_OxygenLevel();
        oxygen_dig1 = getOxygen_Dig1Lvl();
        //if (oxygenLevel <= 25 || oxygenLevel >= 75){
        if (oxygen_dig1>=9){    // motor runs from >80, and runs when 19
            MOTOR = 1;
            pumpCO2LED();
        }else if(oxygen_dig1<8){
            MOTOR = 1;
            pumpOxygenLED();
        }
        else{
            MOTOR = 0;
            RED1 = 0;
            YELLOW1 = 0;
        }
        
        if ((curr_secs%10)==5) { //run motor to change water
            MOTOR = 1;
        } 
        else { 
            MOTOR = 0;
        }
        checkInputMenu();
        if (selection%2==0){           //Display temp
            lcdCtrl_SetPos(2,1);
            for (i=0; temp[i]!=0;i++){
                lcdWriteDspData(temp[i]);
            }

            lcdCtrl_SetPos(2,15);
            for (i=0; percentage[i]!=0;i++){
                lcdWriteDspData(percentage[i]);
            }
            dspTask_OnLCD_Temp();
            temperature = getTemp();
            if(temperature<25){
                RED2 = 1;
            }else if(temperature>29){
                YELLOW2 = 1;
            }else{
                RED2 = 0;
                YELLOW2 = 0;
            }
        }else if (selection%2==1){                      //Display pH
            lcdCtrl_SetPos(2, 1);
            for(i=0; phLvlMsg[i]!=0; i++){
                lcdWriteDspData(phLvlMsg[i]);
            }
            dspTask_onLCD_pHLevel();
            phLvl = getPHLvl();
            if(phLvl<7){
                for(int count=0; count<1000; count++){
                BUZZER = 1;
                __delay_us(500);
                BUZZER = 0;
                __delay_us(500);
                }
            }else if(phLvl>8){
                for(int count=0; count<1000; count++){
                BUZZER = 1;
                __delay_us(500);
                BUZZER = 0;
                __delay_us(500);
                }
            }else{
                BUZZER = 0;
            }
        }
    }
}
//Check with intervals for phLevel
void pumpOxygenLED(void){
    RED1 = 1;
    for(int count=0; count<1000; count++){
        BUZZER = 1;
        __delay_us(250);
        BUZZER = 0;
        __delay_us(250);
    }
    __delay_ms(500);
    RED1 = 0;
    __delay_ms(500);
}

void pumpCO2LED(void){
    YELLOW1 = 1;
    for(int count=0; count<1000; count++){
        BUZZER = 1;
        __delay_us(250);
        BUZZER = 0;
        __delay_us(250);
    }
    __delay_ms(500);
    YELLOW1 = 0;
    __delay_ms(500);
}

void initSysPins(void) {
    ANSELA = 0b00000111;
    ANSELB = 0;
    ANSELC = 0b00000000;
    ANSELD = 0b00000000;
    ANSELE = 0b00000100;
    TRISA = 0b11000011;
    TRISB = 0b11100111;
    TRISC = 0b00000000;
    TRISD = 0b00000011;
    TRISE = 0b11111100;  
}

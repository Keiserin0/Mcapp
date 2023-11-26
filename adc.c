/*
 * File:   LM35.c
 * Author: Sovi
 *
 * Created on 30 June, 2023, 11:56 PM
 */


#include <xc.h>
#include "config.h"

void initADC(void)
{
    ADREF = 0b00000000;
    ADCLK = 0b00000011;
    ADACQ = 0b00000000;
    ADCON0 = 0b10000100;
}

unsigned int adc_GetConversion_Temp(void)
{
    unsigned int result;
    ADPCH = 0b00000000;
    __delay_us(2);
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO == 1);
    result = ((ADRESH*256 + ADRESL)/1023.0)*5*100;
    return(result);
}

unsigned int adc_GetConversion_pH(void){
    unsigned int result;
    unsigned int pHLvl;
    ADPCH = 0b00000001;
    __delay_us(2);
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO);
    result = (ADRESH * 256) + ADRESL;
    pHLvl = (result/1023.0)*14;
    return pHLvl;
}
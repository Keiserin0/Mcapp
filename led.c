/*
 * File:   led.c
 * Author: weiye
 *
 * Created on July 3, 2023, 9:56 AM
 */


#include <xc.h>
#include "config.h"

void onLED(unsigned int value);
 

void onLED(unsigned int result){
    if (result<400 || result>800){
        PORTBbits.RB3 =1;
    }
    else{
        PORTBbits.RB3 =0;
    }
    
}

/*
 * File:   sev_seg.c
 * Author: osovi
 *
 * Created on 7 July, 2023, 10:08 AM
 */


#include <xc.h>
#include "config.h"

unsigned int oxygen_dig1;
const unsigned char segTable[11] = 
{
    0b11000000,
    0b11111001,
    0b10100100,
    0b10110000,
    0b10011001,
    0b10010010,
    0b10000010,
    0b11111000,
    0b10000000,
    0b10010000,
    0b11111111
};

void seg_DispAll(unsigned int result)
{
    unsigned int dig0;
    
    dig0 = result%10;
    oxygen_dig1 = (result/10)%10;
    
    PORTAbits.RA4 = 1;
    PORTAbits.RA5 = 0;
    PORTC = segTable[dig0];
    __delay_ms(5);
    PORTC = segTable[10];
    
    PORTAbits.RA4 = 0;
    PORTAbits.RA5 = 1;
    PORTC = segTable[oxygen_dig1];
    __delay_ms(5);
    PORTC = segTable[10];
    
}

unsigned int getOxygen_Dig1Lvl(void) {
    return oxygen_dig1;
}
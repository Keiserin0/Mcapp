#include <xc.h>
#include "config.h"

#define SW0 PORTBbits.RB0
#define SW1 PORTBbits.RB1
#define MENU PORTDbits.RD1

void lcdWriteDspData(char x);
void lcdCtrl_SetPos(char row, char col);

extern unsigned int oxygenLevel; //initialise waterLevel to 79;
int selection = 0;
unsigned int curr_secs;


unsigned int getOxygenLvl(void){
    return oxygenLevel;
}

void checkInputMenu(void){
    if ((curr_secs%10)==0){
        selection = 1;
        lcdCtrl_SetPos(2,1);
        for(int i=0; i<16; i++){
            lcdWriteDspData(' ');
        }
    } else if((curr_secs%5)==0){
        selection = 0;
        lcdCtrl_SetPos(2,1);
        for(int i=0; i<16; i++){
            lcdWriteDspData(' ');
        }
    }
}
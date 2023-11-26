
#include <xc.h>
#include "config.h"

#define LCD_DATA_D7         PORTDbits.RD7
#define LCD_DATA_D6         PORTDbits.RD6
#define LCD_DATA_D5         PORTDbits.RD5
#define LCD_DATA_D4         PORTDbits.RD4
#define LCD_RS              PORTEbits.RE1   	// RS signal for LCD
#define LCD_E               PORTEbits.RE0   	// E signal for LCD 

// Function Declarations:
// - In this file:
void lcdWriteCtrlWord(char x);
void lcdWriteDspData(char x);
void lcdWriteNibble(char nibble); 
void lcdCtrl_SetPos(unsigned char row, unsigned char col);

void lcdCtrl_FunctionSet(void);
void lcdCtrl_OnOffDisplay(char display_state, char cursor_state);
void lcdCtrl_SetEntryMode(void);
void lcdCtrl_ClearDisplay(void);

void initLCD(void)                      
{				 
    // Special Sequence a) to d) required for 4-bit interface 
    __delay_ms(15);         	   //  a) 15ms LCD power-up delay
    lcdWriteCtrlWord(0b00000011);  //  b) Function Set (DB4-DB7:8-bit interface)
    __delay_ms(5);		   //  c) 5ms delay
    lcdWriteCtrlWord(0b00000010);  //  d) Function Set (DB4-DB7:4-bit interface)    
	
    lcdCtrl_FunctionSet(); 	   //  Function Set - 4-bit, 2 lines, 5x7
    lcdCtrl_OnOffDisplay(1, 0);    //  Display on, cursor off
    lcdCtrl_SetEntryMode();	   //  Entry mode - inc addr, no shift
    lcdCtrl_ClearDisplay();	   //  Clear display & home position
}

void lcdWriteCtrlWord(char x) 
{     
    unsigned char upper_nibble, lower_nibble;
    
    upper_nibble = (x & 0b11110000) >> 4;
    lower_nibble = x & 0b00001111;
    
    LCD_RS = 0;
    lcdWriteNibble(upper_nibble);
    lcdWriteNibble(lower_nibble);
}

void lcdWriteDspData(char x) 
{     
    unsigned char upper_nibble, lower_nibble;
    
    upper_nibble = (x & 0b11110000) >> 4;
    lower_nibble = x & 0b00001111;
    
    LCD_RS = 1;
    lcdWriteNibble(upper_nibble);
    lcdWriteNibble(lower_nibble);
}

void lcdWriteNibble(char nibble) 
{
    LCD_DATA_D7 = (nibble & 0b00001000) >> 3;
    LCD_DATA_D6 = (nibble & 0b00000100) >> 2;
    LCD_DATA_D5 = (nibble & 0b00000010) >> 1;
    LCD_DATA_D4 = (nibble & 0b00000001);
    
    LCD_E       = 1;
    __delay_ms(1);
    LCD_E       = 0;
    __delay_ms(1);    
}

// row values are 1 to 2:
// col values are 1 to 16:
void lcdCtrl_SetPos(unsigned char row, unsigned char col) 
{
    unsigned char ramAddr;        // Ctrl instruction to be sent

    if (row == 1)                 // If row is 1:
        ramAddr = col - 1;        //   Subtract 1 from the col
    else                          // If row is 2:
        ramAddr = 0x40 + col - 1; //   Add 0x40 to ramAddr, and
                                  //   subtract 1 from the col

    lcdWriteCtrlWord(0x80 + ramAddr); // Add 0x80 to ramAddr and write 
                                  // ctrl instruction 
}

void lcdCtrl_FunctionSet(void) 
{
    lcdWriteCtrlWord(0b00101000);
}

void lcdCtrl_OnOffDisplay(char display_state, char cursor_state) 
{
    char pattern = 0b00001000;

    if(display_state == 1)
        pattern |= 0b00000100;    
    if(cursor_state == 1)
        pattern |= 0b00000011;

    lcdWriteCtrlWord(pattern);
}

void lcdCtrl_SetEntryMode(void) 
{
    lcdWriteCtrlWord(0b000001100);    
}

void lcdCtrl_ClearDisplay(void) 
{
    lcdWriteCtrlWord(0b00000001);    
}
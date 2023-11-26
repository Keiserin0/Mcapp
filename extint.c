#include <xc.h>
#include "config.h"

void initSysExtInt(void){
    INTCONbits.GIE = 0;
    PIR0bits.INTF = 0;
    PIE0bits.IOCIE = 0;
    IOCBFbits.IOCBF0 = 0;
    IOCBPbits.IOCBP0 = 1;
    IOCBFbits.IOCBF1 = 0;
    IOCBPbits.IOCBP1 = 1;
    INTPPS = 0x0D;
    INTCONbits.INTEDG = 1;
    PIE0bits.INTE = 1;
    PIE0bits.IOCIE = 1;
    INTCONbits.GIE = 1;
}
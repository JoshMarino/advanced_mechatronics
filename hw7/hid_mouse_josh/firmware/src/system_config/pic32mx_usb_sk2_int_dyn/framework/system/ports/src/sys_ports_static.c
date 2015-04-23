#include "system_config.h"
#include "peripheral/ports/plib_ports.h"


void SYS_PORTS_Initialize(void)
{  
    /* PORT B Initialization */
    ANSELBbits.ANSB13 = 0;      // 0 for digital, 1 for analog
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  0b10000000000000);
    
}
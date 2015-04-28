#include "system_config.h"
#include "peripheral/ports/plib_ports.h"


void SYS_PORTS_Initialize(void)
{
    /* AN and CN Pins Initialization */
//    PLIB_PORTS_AnPinsModeSelect(PORTS_ID_0, SYS_PORT_AD1PCFG, PORTS_PIN_MODE_DIGITAL);
//    PLIB_PORTS_CnPinsPullUpEnable(PORTS_ID_0, SYS_PORT_CNPUE);
//    PLIB_PORTS_CnPinsEnable(PORTS_ID_0, SYS_PORT_CNEN);
//    PLIB_PORTS_ChangeNoticeEnable(PORTS_ID_0);
    
    
    /* PORT B Initialization */
    //USER Button B13
    ANSELBbits.ANSB13 = 0;      // 0 for digital, 1 for analog
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  0b10000000000000);

    //Red LED B3
    ANSELBbits.ANSB3 = 0;      // 0 for digital, 1 for analog
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  0b1000);

    //Yellow LED B7
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  0b10000000);

    //Green LED B15
    ANSELBbits.ANSB15 = 0;      // 0 for digital, 1 for analog
    PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  0b1000000000000000);
    
}

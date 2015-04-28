#include "system_config.h"
#include "system_definitions.h"
#include "peripheral/osc/plib_osc.h"


void SYS_CLK_Initialize( const SYS_CLK_INIT const * clkInit )
{
    SYS_DEVCON_SystemUnlock ( );
    
    PLIB_OSC_FRCDivisorSelect( OSC_ID_0, OSC_FRC_DIV_2);
    /* Enable Peripheral Bus 1 */
    PLIB_OSC_PBClockDivisorSet (OSC_ID_0, 0, 1 );


    SYS_DEVCON_SystemLock ( );
}


inline uint32_t SYS_CLK_SystemFrequencyGet ( void )
{
    return SYS_CLK_FREQ;
}

inline uint32_t SYS_CLK_PeripheralFrequencyGet ( CLK_BUSES_PERIPHERAL peripheralBus )
{
    return SYS_CLK_BUS_PERIPHERAL_1;
}


inline uint32_t SYS_CLK_ReferenceClockFrequencyGet ( CLK_BUSES_REFERENCE referenceBus )
{
	return 0;
}
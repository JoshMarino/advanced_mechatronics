#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes

#include "i2c_master_int.h"
#include "i2c_display.h"
#include "accel.h"

#include <stdio.h>
#include <xc.h>
#include <sys/attribs.h> // __ISR macro


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );



    // set up Vcc LED pin (A4) as digital output
    TRISAbits.TRISA4 = 0;      // 0 is output, 1 is input

    // delay 0.1 seconds after turning on
    LATAbits.LATA4 = 1;
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 4000000) {
        ;
    }

    // Turn on I2C_1 and initialize display
    I2C1CONbits.ON = 1;
    display_init();

    // Setup SPI and initialize accelerometer
    acc_setup();




    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/


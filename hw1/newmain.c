/* 
 * File:   newmain.c
 * Author: josh
 *
 * Created on March 31, 2015, 10:10 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>          // processor SFR definitions
#include <sys/attribs.h> // __ISR macro

// DEVCFGs here - Standalone Mode
#pragma config DEBUG = OFF          // Background Debugger disabled
#pragma config FPLLMUL = MUL_20     // PLL Multiplier: Multiply by 20
#pragma config FPLLIDIV = DIV_4     // PLL Input Divider:  Divide by 4
#pragma config FPLLODIV = DIV_1     // PLL Output Divider: Divide by 1
#pragma config FWDTEN = OFF         // WD timer: OFF
#pragma config POSCMOD = HS         // Primary Oscillator Mode: High Speed xtal
#pragma config FNOSC = PRIPLL       // Oscillator Selection: Primary oscillator w/ PLL
#pragma config FPBDIV = DIV_1       // Peripheral Bus Clock: Divide by 1
#pragma config BWP = OFF            // Boot write protect: OFF
#pragma config ICESEL = ICS_PGx1    // ICE pins configured on PGx1, Boot write protect OFF.
#pragma config FSOSCEN = OFF        // Disable second osc to get pins back
//#pragma config FSRSSEL = PRIORITY_7 // Shadow Register Set for interrupt priority 7


int readADC(void);


int main() {

    // Startup
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that
    // kseg0 is cacheable (0x3) or uncacheable (0x2)
    // see Chapter 2 "CPU for Devices with M4K Core"
    // of the PIC32 reference manual
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // no cache on this chip!

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
    DDPCONbits.JTAGEN = 0;

    __builtin_enable_interrupts();




    // set up USER pin (B13) as digital input
    ANSELBbits.ANSB13 = 0;      // 0 for digital, 1 for analog
    TRISBbits.TRISB13 = 1;      // 0 is output, 1 is input


    // set up YELLOW LED1 pin (B7) as a digital output
    TRISBbits.TRISB7 = 0;       // 0 is output, 1 is input
    LATBbits.LATB7 = 0;


    // set up GREEN LED2 (B15) to OC1 using Timer2 at 1kHz
    RPB15Rbits.RPB15R = 0b0101;     // set B15 to OC1

    OC1CONbits.OCTSEL = 0;   // Select Timer2 for comparison
    T2CONbits.TCKPS = 0x2;   // Timer2 prescaler 1:4
    PR2 = 19999;             // period = (PR2+1) * N * 12.5 ns = 1 kHz
    TMR2 = 0;                // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110;  // PWM mode with no fault pin; other OC1CON bits are defaults

    OC1RS = 0;               // duty cycle = OC1RS/(PR3+1) = 0%
    OC1R = 0;                // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1


    // set up a potentiometer and read the output voltage on some analog input pin, AN0 as A0
    ANSELAbits.ANSA0 = 1;       // 0 for digital, 1 for analog
    AD1CON3bits.ADCS = 3;       // ADC Conversion Clock Select bit
    AD1CHSbits.CH0SA = 0;       // Channel 0 positive input is AN0 (0)
    AD1CON1bits.ADON = 1;       // ADC module is operating (1)


    _CP0_SET_COUNT(0);

    while (1) {
        //Toggle LED1 every 1/2 second
	if (_CP0_GET_COUNT() > 10000000) {
            LATBINV = 0x80;
            _CP0_SET_COUNT(0);
	}

        //When USER is pushed, toggle LED1 as fast as possible
        if (!PORTBbits.RB13) {
            LATBINV = 0x80;
        }

        //Set LED2 brightness proportional to the potentiometer voltage
        int pot_voltage = readADC();
        int brightness = ((int) (((float)pot_voltage)*20000/1024) );
        OC1RS = brightness;

        //Set LED2 frequency to 1kHz using Timer 2 and verify with the nScope or Tek scope
        
    }
}

int readADC(void) {
    int elapsed = 0;
    int finishtime = 0;
    int sampletime = 20;
    int a = 0;

    AD1CON1bits.SAMP = 1;
    elapsed = _CP0_GET_COUNT();
    finishtime = elapsed + sampletime;

    while (_CP0_GET_COUNT() < finishtime) {
    }

    AD1CON1bits.SAMP = 0;

    while (!AD1CON1bits.DONE) {
    }

    a = ADC1BUF0;
    return a;
}
#include "accel.h"
#include <xc.h>
// interface with the LSM303D accelerometer/magnetometer using spi
// Wire GND to GND, VDD to 3.3V, 
// output pin SDO1      (pin RPB2, 5) -> SDI (labeled SDA)
// input pin SDI1       (pin RPB1, 5) -> SDO
// SCK1 	        (pin B14, 25) -> SCL
// some digital pin	(pin RB5, 14) -> CS
// do not use pin 21 or 22


#define CS LATBbits.LATB4 // replace x with some digital pin

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


// read data from the accelerometer, given the starting register address.
// return the data in data
void acc_read_register(unsigned char reg, unsigned char data[], unsigned int len) {
  unsigned int i;
  reg |= 0x80; // set the read bit (as per the accelerometer's protocol)
  if(len > 1) {
    reg |= 0x40; // set the address auto increment bit (as per the accelerometer's protocol)
  }
  CS = 0;
  spi_io(reg);
  for(i = 0; i != len; ++i) {
    data[i] = spi_io(0); // read data from spi
  }
  CS = 1;
}


void acc_write_register(unsigned char reg, unsigned char data) {
  CS = 0;               // bring CS low to activate SPI
  spi_io(reg);
  spi_io(data);
  CS = 1;               // complete the command
}


void acc_setup() {

  // turn off AN10 to be able to use SCK1 pin
  ANSELBbits.ANSB14 = 0;     // 0 for digital, 1 for analog

  // set CS (B4) to output and digital if necessary
  TRISBbits.TRISB4 = 0;      // 0 is output, 1 is input
  CS = 1;                    // set CS to high, not reading/writing anything

  // set SDI_1 to pin RPB5 -> SDO
  ANSELBbits.ANSB1 = 0;      // 0 for digital, 1 for analog
  SDI1Rbits.SDI1R = 0b0001;

  // set SDO_1 to pin RPB2 -> SDA
  ANSELBbits.ANSB2 = 0;      // 0 for digital, 1 for analog
  RPB2Rbits.RPB2R = 0b0011;  



  // Setup the master Master - SPI1
  // we manually control SS as a digital output 
  // since the pic is just starting, we know that spi is off. We rely on defaults here
 
  // setup SPI1
  SPI1CON = 0;              // turn off the spi module and reset it
  SPI1BUF;                  // clear the rx buffer by reading from it
  SPI1BRG = 0x3;            // baud rate to 5MHz [SPI1BRG = (40000000/(2*desired))-1]
  SPI1STATbits.SPIROV = 0;  // clear the overflow bit
  SPI1CONbits.CKE = 1;      // data changes when clock goes from active to inactive
                            //    (high to low since CKP is 0)
  SPI1CONbits.MSTEN = 1;    // master operation
  SPI1CONbits.ON = 1;       // turn on spi
 
  // set the accelerometer data rate to 1600 Hz. Do not update until we read values
  acc_write_register(CTRL1, 0xAF);

  // 50 Hz magnetometer, high resolution, temperature sensor on
  acc_write_register(CTRL5, 0xF0);

  // enable continuous reading of the magnetometer
  acc_write_register(CTRL7, 0x0);

  // CTRL2 register: AFS default value: 0
  // address: 0x21 or 0100001
  // register bits: ABW1, ABW0, AFS2, AFS1, AFS0, 0, AST, SIM (default 00000000)
  // acceleration +/- 2g -> AFS2=0, AFS1=0, AFS0=0
  acc_write_register(CTRL2, 0x0);
}

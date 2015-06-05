Advanced Mechatronics
=============================================

Josh Marino 
---------------------------------------------


#### Table of Contents ####
1. [Homework #1](#hw1)
2. [Homework #2](#hw2)
3. [Homework #3](#hw3)
4. [Homework #4](#hw4)
5. [Homework #5](#hw5)
6. [Homework #6](#hw6)
7. [Homework #7](#hw7)
8. [Homework #8](#hw8)
9. [Homework #9](#hw9)
10. [Homework #10](#hw10)
11. [Homework #11](#hw11)
12. [Homework #14](#hw14)
13. [Tech Cup](#tech_cup)


#### Homework #1 <a name="hw1"></a>
The first homework assignment included wiring the PIC32 with 2 LEDS, 2 push buttons, and a potentiometer. Code had to be written for the following tasks:

1. Toggle LED1 every 1/2 second
2. When USER is pushed, toggle LED1 as fast as possible
3. Set LED2 brightness proportional to the potentiometer voltage
4. Set LED2 frequency to 1kHz using Timer 2 and verify with the nScope or Tek scope


#### Homework #2 <a name="hw2"></a>
Our second homework assignment involved created an EAGLE schematic for our protoboard. New devices had to be created for the following:

1. PIC32MX250F128B (Digikey ED3026-ND)
2. 8MHz resonator (Digikey X905-ND)
3. 2-lead push button (Digikey P8072SCT-ND)

Last, the chips were added to the schematic, and components were connected together with electrical connections.


#### Homework #3 <a name="hw3"></a>
The third homework assignment created a board layout from the EAGLE schematic for our protoboard. After the components were placed, connections were added for wires. Once everything was wired and connected, gerber files were generated and double-checked using mayhewlabs 3d gerber viewer.


#### Homework #4 <a name="hw4"></a>
The fourth homework assignment was interfacing with the (SSD1306 128x64 I2C OLED display)[https://www.adafruit.com/products/326]. We had to write code for a driver that would show text on the display screen. This was primarily done using display_pixel_set() and draw_display() in which each pixel was set to either 0 or 1, and only appeared on the display after calling draw_display().

![test_display](hw4/IMG_0925.JPG)


#### Homework #5 <a name="hw5"></a>
The fifth homework assignment was interfacing with the (LSM303D accelerometer)[https://www.pololu.com/product/2127]. SPI communication was used to obtain the accelerometer, magnetometer, and temperature sensor readings. We then used the display from HW #4 display bars extending from the center of the OLED display, with a length proportional to the X and Y acceleration due to gravity.

![test_display](hw5/acceleration_values.JPG)


#### Homework #6 <a name="hw6"></a>
The sixth homework assignment was to get the Harmony project "blinky_leds" working on our board.


#### Homework #7 <a name="hw7"></a>
The seventh homework assignment was to get the Harmony project "hid_mouse" working on our board. This interfaced with our USB breakout board and PIC32, while creating a circle for a path for a mouse. However, we modified the x and y positions of the mouse to interface with the accelerometer values.


#### Homework #8 <a name="hw8"></a>
Get the hid_basic Harmony example to compile with the PIC32MX250F128B. Setup the LEDs on your board to tell you the USB state (used in void APP_USBDeviceEventHandler). Follow the instructions on the Generic HID page to get the HIDAPI to work with the Harmony example. Every time you run hidtest, the PIC should toggle some LEDs and return the state of the button.

Change the code to complete the following task: When the computer program runs, the user enters a string and a row number, the computer sends the data to the PIC, and the PIC displays the string on the OLED display on the correct row. After displaying the string, the PIC samples the accelerometer x, y, and z data at 100Hz and sends it to the computer. The computer saves the data into a text file with data in 3 space-separated columns.


#### Homework #9 <a name="hw9"></a>
Design a box to lasercut out of 1/8" thick plywood. Use a tabbed design, with t-slots for 6-32 screws and nuts. The box should have 4 parts (one of them flexible) with an open top. Also, design a wheel to 3D print on one of the printers in the mechatronics lab. The wheel should fit a 2" silicone O-ring tire, McMaster part number 9396K83. The wheel should press fit onto a D-shape motor output shaft, 3mm in diameter, 2.5mm from edge to flat.

![test_display](hw9/assembly.png)
![test_display](hw9/wheel.png)


#### Homework #10 <a name="hw10"></a>
Test the Hello World code built into Android Studio. Follow the instructions at the Android Developer page to make your first app. Go through "Starting Another Activity."


#### Homework #11 <a name="hw11"></a>
Edit HW #8 to send only the Z acceleration data at 500Hz for 20 seconds. Take data from the accelerometer while shaking the board up and down at 1Hz. Plot the data in MATLAB, and perform an FFT. Verify that there is a spike at 1Hz.

Create a MAF to smooth the data using a 5 element FIFO. Send the filtered acceleration back to the computer along with the unfiltered. Plot both and perform an FFT on both, and note how well the MAF worked.

Pick a low-pass frequency that would remove the noise but keep the 1Hz shaking. Use MATLAB to design FIR coefficients to reduce noise above that frequency. Use enough coefficients to avoid frequency ripple, but no more than 12. Implement the FIR, and send the filtered data back to the computer with the unfiltered and MAF filtered data. Plot vs time and as an FFT in MATLAB.


#### Homework #14 <a name="hw14"></a>
Robot CAD files for completing a lap while following a line.
![test_display](hw14/car_assembly.png)


#### Tech Cup <a name="tech_cup"></a>
In order to complete a lap for the Tech Cup, an app needed to be created for the Android phone
1. Android app consists of communication over CDC serial port from Android phone to PIC, as well as image processing of the camera stream to find the center of mass (COM) of a line.
2. PIC code receives the COM from CDC serial port over USB and tells the motors to operate accordingly.

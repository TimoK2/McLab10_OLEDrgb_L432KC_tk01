/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
************************************************************************
*
* Commissioning test for the L432KC and the Pmod small OLED display
*
*************************************************************************
* Description: McLab10_OLEDrgb_L432KC_OS60_tk2
* "Test module Pmod Digilent Lextronic" will be displayed on OLEDrgb module
* with different size and colors..... and later the variable values.
*
* Material
* 1. ST L432KC  ( or some other micro controller board with SPI communication )
* 2. Digilent Pmod OLEDrgb  and the libraries
* Adafruit_SSD1331_Mbed
* Adafruit-GFX
* These libraries can be found with search on the page 
* https://www.mbed.com/en/platform/mbed-os/
* Please connect L432KC - Pmod_OLEDrgb with lines:
* L432KC D13 - OLED 4 SCK   hardware defined for the SPI
* L432KC D11 - OLED 2 MOSI  hardware definef for the SPI
* L432KC A7  - OLED 1 CS   or any other free
* L432KC D10 - OLED 7 DC   or any other free
* L432KC D6  - OLED 8 RES   or any other free
*  VCC     - OLED 9 VCCEN  Enable - VCC
*  VCC     - OLED10 PMODEN  Pmod Enable - VCC
*  GND     - OLED 5 Ground
*  VCC     - OLED 6 Power supply 3.3 V
* L432KC 3V3 - VCC  L432 supplying 3.3 V
* L432KC GND - GND  Ground
* 
*************************************************************************
* Updated for OS6.7, OS6.8 
* Timo Karppinen 23.02.2021  SPDX-License-Identifier: Apache-2.0
            OS 6.15 compiles OK 8.1.2022
**************************************************************/

#include "mbed.h"
#include "Adafruit_SSD1331.h"   // By using the Adafruit SSD1331 library and Adafruit GFX library
#include "Adafruit_GFX.h"       // we will get similar code working than in Arduino-boards.
                                // There are other SSD1331 libraries, too. 
                                // https://os.mbed.com/search/?q=ssd1331  
                                // The tested are:
                                // Adafruit_SSD1331_MbedOS6 by Timo Karppinen
                                // Adafruit-GFX-MbedOS6 by Timo Karppinen             
// PmodOLEDrgb
Adafruit_SSD1331 OLED(A7, D6, D10, D11, NC, D13); // cs, res, dc, mosi, (nc), sck  


DigitalOut LED(D1);     // LED1, LED2, LED3 and LED4 are the D13 PB_3 pin in this board and 
                            //can not be used as a LED because of the SPI
DigitalOut VCCEN(D3);
DigitalOut PMODEN(D5);

// Definition of colours on the OLED display
#define Black 0x0000
#define Blue 0x001F
#define Red 0xF800
#define Green 0x07E0
#define Cyan 0x07FF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define White 0xFFFF

float ADCScaledF = 0;       // 32 bit floating point
int volt = 0;
int millivolt = 0;
void getADC();

char Time[32];

void getTime();
int first = 0;

int main()
{
  // Showing with a LED that program has started
        LED = 0;
        VCCEN = 1;    // if you did not connect VCCEN permanently to Vcc
        PMODEN = 1;   // if you did not connect PMODEN permanently to Vcc
        ThisThread::sleep_for(2000ms);
        LED = 1;
        ThisThread::sleep_for(2000ms);
        LED = 0;
  // Setting the time
  set_time(1614012790);  // Set RTC time to " https://www.epochconverter.com/ "     
  // Initalize the PmodOLEDrgb, the library includes SPI and there is
  // no need to initialize the SPI in the main.cpp   
        OLED.begin(); // initialization of display object
        OLED.clearScreen();   

    while (true) {
        while(first < 3)
            {
                first += 1; 
                OLED.fillScreen(Black); // background screen in black
                OLED.setTextColor(Cyan); // colour of text in cyan
                OLED.setCursor(0,0); // cursor is in x=0 and y=15
                OLED.printf("Test module Pmod"); // display text
                ThisThread::sleep_for(500ms); // wait 500 ms
                OLED.setCursor(0,15); // cursor is in x=0 and y=15
                OLED.setTextSize(2); // size of text
                OLED.setTextColor(Red); // text in red colour
                OLED.printf("DIGILENT"); // display text
                OLED.setCursor(20,40); // cursor is in x=20 and y=40
                OLED.setTextSize(1); // size of text
                OLED.setTextColor(Green); // text in green colour
                OLED.printf("LEXTRONIC"); // display text
                OLED.drawFastHLine(1, 60, OLED.width()-1, Blue); //blue line x=1, width-1 and y=60
                ThisThread::sleep_for(2s); // wait 2 s
                OLED.fillScreen(Black); // background display in black (erase display)
                OLED.fillRoundRect(5, 5, 30, 40, 1, Blue); // French flag bleu blanc rouge
                OLED.fillRoundRect(35, 5, 30, 40, 1, White);
                OLED.fillRoundRect(65, 5, 30, 40, 1, Red);
                OLED.fillCircle(90, 55, 5, Yellow); // yellow circle with radius=5 in x=90 and y=55
                ThisThread::sleep_for(1s); // wait 1 s
            }

       
        //ThisThread::sleep_for(300ms);
        OLED.clearScreen();
        OLED.fillScreen(Blue); // background screen in blue
        OLED.setTextColor(Cyan); // colour of text in cyan
        OLED.setCursor(0,0); // cursor is in x=0 and y=0
        OLED.printf("Rec");
        getTime();
        OLED.printf(" at %s \n",Time);
        getADC(); 
        OLED.printf("VOLT :%d.%03d\r\n", volt, millivolt);

        if (ADCScaledF > 2.70)
        { 
            OLED.setTextColor(Yellow);
            OLED.printf("Be aware of reaching the limit\n");
            OLED.drawFastHLine(10, 43, 50, Yellow);
        }
        OLED.setCursor(0,46); // cursor is in x=0 and y=40
        OLED.printf("Wait a moment !");
        OLED.drawFastHLine(1, OLED.height()-1, OLED.width()-1, Red); //RED line x=1 to screen width-1 and 
                                                                    // y=screen height-1
        //printf("printed on OLED\n\n");
        
        ThisThread::sleep_for(1000ms);
}
}

void getTime()
{    
    time_t seconds = time(NULL);   //  https://os.mbed.com/docs/mbed-os/v6.7/apis/time.html
    strftime(Time,32,"%I:%M:%p\n", localtime(&seconds));
    //printf("Recorded :%s \r\n", Time); // in mbed OS 6.7 interferes with OLED.printf  !!!
   
}

void getADC()
{
    static int count = 200;     // value like it would be from a 12 bit Analog to Digital Converter
   
    count = count + 82;
    if (count > 4095){
        count = 200;
        }
    
    ADCScaledF = (float(count))*(float(3.3)/4095); // The 0.00 V to  3.30 V corresponds 32 bit floating point.
    volt = (int)ADCScaledF;                     // The OS6 does not include floating point printf !!!
    millivolt = ((int)(ADCScaledF*1000))% 1000; // Millivolts counted with c++ modulus operator
    //printf("ADC   :%d \r\n",count);   // in mbed OS 6.7 interferes with OLED.printf  !!!
    //printf("VOLT  :%d.%03d\r\n", volt, millivolt); // millivolts with preceeding zeros and three digits
    ThisThread::sleep_for(100ms);
}


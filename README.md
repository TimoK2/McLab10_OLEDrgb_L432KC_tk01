# McLab10 OLEDrgb L432KC

The code is developed for the hardware NUCLEO-L432KC and Digilent Pmod OLEDrgb.
The purpose is to make students learn the development process
and test that the hardware works fine.

Built started with empty OS6 project. 
 
For converting this to Mbed OS 6
 
Adafruit_GFX.h
needed to add 
#include "Stream.h"
in beginning of the Adafruit_GFX.h
 
Adafruit_SSD1331.cpp
wait_ms(200)
replaced with 
ThisThread::sleep_for(200ms);
for all the similar wait_ms lines.
 
 
### License and contributions
 
The software is provided under Apache-2.0 license. 
Contributions to this project are accepted under the same license. 
Please see contributing.md for more info.
 
This project contains code from other projects. 
The original license text is included in those source files. 
They must comply with our license guide.
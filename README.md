# TandHSensors
This program reads 5 PT1000 or PT100 sensors and 1 relative humidity HIH400 
sensor using an Arduino board.

ZIP file contains a modified version of the PWFusion_31865 library whose 
original can be found in https://github.com/PlayingWithFusion/PWFusion_MAX31865.
However, original library should not be used in this application. Main 
differences between both is the way in which temperature is calculated from 
resistance. The current algorithm to convert resistance to temperature was taken 
from Adafruit_MAX31865 library which can be downloaded from Arduino's library
manager.

To switch between PT100 and PT1000, RTD_TYPE_PT100 or RTD_TYPE_PT1000 should be 
picked. Also, RNOMINAL should be adjusted to 100.0 or 1000.0 for PT100 or PT1000,
respectively. RNOMINAL can be found in PwFusion_MAX31865.h, line 68.

Be sure that this library is properly installed as described here: 
https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries

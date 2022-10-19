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

Be sure that this library is properly installed as described here: 
https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries

This code can read PT1000 board properly but parameters have to be adjusted 
properly.



/***************************************************************************
* File Name: max31865_2ch_spi_example.ino
* Processor/Platform: Playing With Fusion R3aktor M0 (tested)
* Development Environment: Arduino 1.8.13
*
* Designed for use with with Playing With Fusion MAX31865 Resistance
* Temperature Device (RTD) breakout board: SEN-30202 (PT100 or PT1000)
*   ---> http://playingwithfusion.com/productview.php?pdid=25
*   ---> http://playingwithfusion.com/productview.php?pdid=26
*
* Copyright © 2014 Playing With Fusion, Inc.
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License. 
* 
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
* **************************************************************************
* REVISION HISTORY:
* Author                        Date            Comments
* J. Steinlage                  2014Feb17       Original version
* J. Steinlage                  2014Aug07       Reduced SPI clock to 1MHz - fixed occasional missing bit
*                                               Fixed temp calc to give only unsigned resistance values
*                                               Removed unused #defines for chip config (they're hard coded) 
* J. Leonard                    2021Mar19       Updated to object-oriented MAX31865 library
*
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
*
* **************************************************************************
* ADDITIONAL NOTES:
* This file configures then runs a program on PwFusion R3aktor to read a
* dual channel MAX31865 RTD-to-digital converter breakout board and print results
* to a serial port. Communication is via SPI built-in library.
*    - Configure Arduino Uno or PwFusion R3aktor
*    - Configure and read resistances and statuses from MAX31865 IC 
*      - Write config registers (MAX31865 starts up in a low-power state)
*      - RTD resistance register
*      - High and low status thresholds 
*      - Fault statuses
*    - Write formatted information to serial port
*  Circuit:
*    Arduino Uno   Arduino Mega   R3aktor M0      -->  SEN-30202
*    CS0:  pin  9  CS0:  pin  9   CS0: pin D9     -->  CS, CH0
*    CS1:  pin 10  CS1:  pin 10   CS0: pin D10    -->  CS, CH1
*    MOSI: pin 11  MOSI: pin 51   MOSI: pin MOSI  -->  SDI (must not be changed for hardware SPI)
*    MISO: pin 12  MISO: pin 50   MISO: pin MISO  -->  SDO (must not be changed for hardware SPI)
*    SCK:  pin 13  SCK:  pin 52   SCK:  pin SCK   -->  SCLK (must not be changed for hardware SPI)
*    GND           GND            GND             -->  GND
*    5V            5V             3.3V            -->  Vin (supply with same voltage as Arduino I/O, 5V or 3.3V)
***************************************************************************/

// Declaring if the instrumentation would have a LCD screen.
// If setup won't have a LCD just comment next line. 

#define LCDFLAG

// the sensor communicates using SPI, so include the hardware SPI library:
#include <SPI.h>
// include Playing With Fusion MAX31865 library
#include <PwFusion_MAX31865.h> 

#ifdef LCDFLAG
#include <LiquidCrystal.h>
#endif



// CS pin used for the connection with the sensor
// other connections are controlled by the SPI library)
const int CS0_PIN = 10;
const int CS1_PIN = 9;
const int CS2_PIN = 8;
const int CS3_PIN = 7;
const int CS4_PIN = 6;

int HIH4000_Pin = A0;
int HIHAnalogRead = 0;

// Create instance of MAX31865 class
MAX31865 rtd0;
MAX31865 rtd1;
MAX31865 rtd2;
MAX31865 rtd3;
MAX31865 rtd4;

//Defining pins used for LCD

#ifdef LCDFLAG
const int rs = 5, en = 4, d4 = 3, d5 = 2, d6 = A1, d7 = A2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#endif

void setup() {
  Serial.begin(115200);
  Serial.println("Boot");

  // setup for the the SPI library:
  SPI.begin();
  
  // initalize the chip select pin
  pinMode(CS0_PIN, OUTPUT);
  pinMode(CS1_PIN, OUTPUT);
  pinMode(CS2_PIN, OUTPUT);
  pinMode(CS3_PIN, OUTPUT);
  pinMode(CS4_PIN, OUTPUT);
  #ifdef LCDFLAG
  pinMode(d6,OUTPUT);
  pinMode(d7,OUTPUT);
  #endif
  // configure rtd sensor channel 0
  rtd0.begin(CS0_PIN, RTD_2_WIRE, RTD_TYPE_PT100);

  // configure rtd sensor channel 1
  rtd1.begin(CS1_PIN, RTD_2_WIRE, RTD_TYPE_PT100);

  // configure rtd sensor channel 2
  rtd2.begin(CS2_PIN, RTD_2_WIRE, RTD_TYPE_PT100);

  // configure rtd sensor channel 3
  rtd3.begin(CS3_PIN, RTD_2_WIRE, RTD_TYPE_PT100);  
  
  // configure rtd sensor channel 3
  rtd4.begin(CS4_PIN, RTD_2_WIRE, RTD_TYPE_PT100);  

  // Serial.println(F("MAX31865 Configured"));
  
  // give the sensor time to set up
  delay(100);

  #ifdef LCDFLAG
  lcd.begin(16,2);
  lcd.clear();
  #endif
  
}

void loop() 
{
  // Get the latest temperature from the MAX31865
  rtd0.sample();
  rtd1.sample();
  rtd2.sample();
  rtd3.sample();
  rtd4.sample();
  float t0 = (float)rtd0.getTemperature();
  float t1 = (float)rtd1.getTemperature();
  float t2 = (float)rtd2.getTemperature();
  float t3 = (float)rtd3.getTemperature();
  float t4 = (float)rtd4.getTemperature();
  
  // Get the average of n=10 measurments of relative humidity
  float RH = getRelativeHumidityAverage(A0,25,10);

  // Prints the measurements obtained by sensors
  printMeasurmentToSerial(t0,t1,t2,t3,t4,RH);

  #ifdef LCDFLAG 
  printMeasurementToLCD(lcd, t0,t1,t2,t3,t4,RH);
  #endif
  delay(1000);    
}

void PrintRTDStatus(uint8_t status)
{
  // status will be 0 if no faults are active
  if (status == 0)
  {
    Serial.print("OK");
  }
  else 
  {
    // status is a bitmask, so multiple faults may be active at the same time

    // The RTD temperature is above the threshold set by setHighFaultTemperature()
    if (status & RTD_FAULT_TEMP_HIGH)
    {
      Serial.print("RTD High Threshold Met, ");
    }

    // The RTD temperature is below the threshold set by setHLowFaultTemperature()
    if (status & RTD_FAULT_TEMP_LOW)
    {
      Serial.print("RTD Low Threshold Met, ");
    }

    // The RefIn- is > 0.85 x Vbias
    if (status & RTD_FAULT_REFIN_HIGH)
    {
      Serial.print("REFin- > 0.85 x Vbias, ");
    }

    // The RefIn- or RtdIn- pin is < 0.85 x Vbia
    if (status & (RTD_FAULT_REFIN_LOW_OPEN | RTD_FAULT_RTDIN_LOW_OPEN))
    {
      Serial.print("FORCE- open, ");
    }

    // The measured voltage at the RTD sense pins is too high or two low
    if (status & RTD_FAULT_VOLTAGE_OOR)
    {
      Serial.print("Voltage out of range fault, ");
    }
  }

  Serial.println();
}

float getRelativeHumidity(int HIH4000_Pin, float temperature){
  
  float analogicReading = (float)analogRead(HIH4000_Pin);
  float temperatureFactorCorrection = 1/(1.0546-0.00216*temperature);
  float av = (5000.0/31.0)*((analogicReading/1024.0)-0.16);
  float RH = av*temperatureFactorCorrection;
  return RH;
}

float getRelativeHumidityAverage(int HIH4000_Pin, float temperature,int n){
  float sumRH = 0.0;
  for (int i = 0; i < n; i++ ){
    sumRH = sumRH + getRelativeHumidity(HIH4000_Pin,temperature);
  }
  float RH = sumRH/n;
  return RH;
}

void printMeasurmentToSerial(float f0,float f1,float f2,float f3,float f4, float RH){
  Serial.print("t0=");
  Serial.print(f0);
  Serial.print(",t1=");
  Serial.print(f1);
  Serial.print(",t2=");
  Serial.print(f2);
  Serial.print(",t3=");
  Serial.print(f3);
  Serial.print(",t4=");
  Serial.print(f3);
  Serial.print(",RH=");
  Serial.print(RH);
  Serial.print("\n");
}

#ifdef LCDFLAG
void printMeasurementToLCD(LiquidCrystal lcd,float f0,float f1,float f2,float f3,float f4, float RH){
  lcd.setCursor(0,0);
  lcd.print(f0,4);
  lcd.print(",");
  lcd.setCursor(5,0);
  lcd.print(f1,4);
  lcd.print(",");
  lcd.setCursor(10,0);
  lcd.print(f2,4);
  lcd.setCursor(0,1);
  lcd.print(f3,4);
  lcd.print(",");
  lcd.setCursor(5,0);
  lcd.print("RH=");
  lcd.print(RH,4);
}
#endif

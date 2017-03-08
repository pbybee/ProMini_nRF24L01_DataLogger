#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include <string.h>
#include <inttypes.h>
#include <SPI.h>
#include <Wire.h>
#include "nRF24L01.h"
#include "MPL3115A2.h"
#include "QuadCopter.h"
#include "AccelUtil.h"

//Object Instantiation

// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire
MPL3115A2 baro = MPL3115A2();

//Instantiate nRF module Object
//Setup pins
//  10 (SS), 11 (MOSI), 12 (MISO), 13 (SCK)
uint8_t cePin = 9;
uint8_t csnPin = 8;
uint8_t channel = 1;
uint8_t payload = 14; //Set payload of packet in bytes
Nrf24l radio (cePin, csnPin, channel, payload);

long time = 0;

void setup() {
  
  //##################DEBUG###########
  // Serial.begin(9600);
  //############################
  
  //Init SPI for Radio
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_2XCLOCK_MASK);
  
  //Init the radio module
  radio.init();
  radio.setRADDR((byte *)"clie1");
  radio.config();
  radio.setTADDR((byte *)"srvr1");
  
  //Setup the I2C wire
  Wire.begin();
  
  //Startup the Accelerometer
	setupMPU();
  // if (!setupMPU()) {
		// Serial.println("Could not find Accelerometer")
	// return;
  // }
  
  //setup Altimeter
	baro.begin();
  // if (! baro.begin()) {
    // Serial.println("Could not find Pressure Sensor");
    // return;
  // }
  
  //Start the timer
  time = millis();
}


void loop()
{
		uint16_t currentPressure = 0;
		
		//Get currentPressure in kPa
		currentPressure = baro.getPressure();
		
		//Get Accel Data
		readData();
		
		//Transmit Pressure and Accel Data
		//Re-reference the integer arrays (16bit) to byte size SPI transmissions
		uint8_t array[14] = { 0 };
		uint16_t* intArray = (uint16_t*) array;

		//Into the 16bit referenced intArray, place the sensor readings
		intArray[0] = currentPressure;
		intArray[1] = accelDataX;
		intArray[2] = accelDataY;
		intArray[3] = accelDataZ;
		intArray[4] = gyroDataX;
		intArray[5] = gyroDataY;
		intArray[6] = gyroDataZ;

		//Then Transmit the 8bit referenced array address pointer
		radio.send((byte *) array);
		
		//Wait for radio
		while(radio.isSending()){
		}
}



/*
  External EEPROM Recording & Playback Demo
  ext_eeprom_demo.ino
  Uses AT24LC256 External I2C EEPROM

  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include the I2C Wire Library
#include "Wire.h"



// EEPROM I2C Address
#define EEPROM_I2C_ADDRESS 0x50

// Analog pin for potentiometer
int analogPin = 28;

// Integer to hold potentiometer value
int val = 0;

// Byte to hold data read from EEPROM
int readVal = 0;

// Integer to hold number of addresses to fill
int maxaddress = 10;





// Function to read from EEPROM
byte readEEPROM(int address, int i2c_address)
{
  // Define byte for received data
  byte rcvData = 0xFF;

  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_address);

  // Send memory address as two 8-bit bytes
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB

  // End the transmission
  Wire.endTransmission();

  // Request one byte of data at current memory address
  Wire.requestFrom(i2c_address, 1);

  // Read the data and assign to variable
  rcvData =  Wire.read();

  // Return the data as function output
  return rcvData;
}


void setup()
{
  // Connect to I2C bus as master
  Wire.begin();

  // Setup Serial Monitor
  Serial.begin(9600);

  // Attach servo on pin 9 to the servo object
  

  // Print to Serial Monitor
  Serial.println("Start Recording...");

  // Run until maximum address is reached

 

  // Print to Serial Monitor
  Serial.println("Begin Playback...");
delay(5000);
  // Run until maximum address is reached

  for (int address2 = 0; address2 <= maxaddress; address2++) {

    // Read value from EEPROM
    readVal = readEEPROM(address2, EEPROM_I2C_ADDRESS);


    // Write to the servo
    // Delay to allow servo to settle in position
    // Convert value to integer for servo
    
    delay(15);

    // Print to Serial Monitor
    Serial.print("ADDR = ");
    Serial.print(address2);
    Serial.print("\t");
    Serial.println(readVal);

  }

  // Print to Serial Monitor
  Serial.println("Playback Finished!");

}

void loop()
{

  // Nothing in loop

}

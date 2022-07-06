#include <Wire.h>
#define eepromAddress 0x50 //i2c Address of EEPROM
byte storageAddress; //limit to 255
void setup() {
  Serial.begin(115200);
  delay(3000);
  int intValue = 9999; 
  Serial.print("16 bit value to store in eeprom = ");
  Serial.println(intValue);
  
 storageAddress = 0;//manage storageAddress
  writeEEPROM(storageAddress, lowByte(intValue), eepromAddress);
  writeEEPROM(storageAddress + 1, highByte(intValue), eepromAddress);
  Serial.print("16 bit value retreived from eeprom = ");
  byte l = readEEPROM(storageAddress, eepromAddress);
  byte h = readEEPROM(storageAddress + 1, eepromAddress);
  int combineInt = word(h,l);
  Serial.println(combineInt);
  
  long longValue = 12345678;
  Serial.print("32 bit value to store in eeprom = ");
  Serial.println(longValue);
  byte byteVal[4];//4 bytes to hold separated values
  byteVal[3] = longValue >> 24;
  byteVal[2] = longValue >> 16;
  byteVal[1] = longValue >> 8;
  byteVal[0] = longValue;

  storageAddress = 16; //manage storage address
  
  for (byte i = 0; i < 4; i++)
  {
    writeEEPROM(storageAddress, byteVal[i], eepromAddress);
    storageAddress++;
  }

  //read data back and recombine
  storageAddress = 16;
  for (byte i = 0; i < 4; i++) 
  {
    byteVal[i] = readEEPROM(storageAddress, eepromAddress);
    storageAddress++;
  }
  
  long recombinedValue = 0;
  recombinedValue = byteVal[3];
  recombinedValue = recombinedValue << 8 | byteVal[2];
  recombinedValue = recombinedValue << 8 | byteVal[1];
  recombinedValue = recombinedValue << 8 | byteVal[0];

  Serial.print("32 bit value retreived from eeprom = ");
  Serial.println(recombinedValue);

}

void loop() {}

// Function to write to EEPROOM
void writeEEPROM(byte address, byte val, int i2c_Address)
{
  Wire.beginTransmission(i2c_Address);
  //Wire.write(address >> 8); not used for small eeprom
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
  delay(5);
}

// Function to read from EEPROM
byte readEEPROM(byte address, int i2c_Address)
{
  byte rcvData = 0xFF;

  Wire.beginTransmission(i2c_Address);
  //Wire.write(address >> 8);//not used for small eeprom
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(i2c_Address, 1);
  rcvData =  Wire.read();
  return rcvData;
}

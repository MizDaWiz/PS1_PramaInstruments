#include <Wire.h>
#define I2C_ADDRESS 0x50

void setup(){
  Wire.begin();
  Serial.begin(9600);
  
  unsigned int address = 280;
  byte byteVal_1 = 42;
  byte byteVal_2 = 123;
  byte byteVal_3 = 111;
  delay(5000);
  writeEEPROM(address,byteVal_1,I2C_ADDRESS);
  address++;
writeEEPROM(address,byteVal_2,I2C_ADDRESS);
  address++;
  writeEEPROM(address,byteVal_3,I2C_ADDRESS);
  
  for(address=280; address<283; address++){
    Serial.print("Byte at address ");
    Serial.print(address);
    Serial.print(": ");
    Serial.println(readEEPROM(address,I2C_ADDRESS));
  }
}
void loop(){}
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

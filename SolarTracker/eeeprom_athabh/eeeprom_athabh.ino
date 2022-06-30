#include <Wire.h>
#define I2C_ADDRESS 0x51

void setup(){
  Wire.begin();
  Serial.begin(9600);
  
  unsigned int address = 0;
  byte byteVal_1 = 42;
  byte byteVal_2 = 123;
  byte byteVal_3 = 255;
  delay(5000);
  eepromByteWrite(address,byteVal_1);
  address++;
  eepromByteWrite(address,byteVal_2);
  address++;
  eepromByteWrite(address,byteVal_3);
  
  for(address=0; address<3; address++){
    Serial.print("Byte at address ");
    Serial.print(address);
    Serial.print(": ");
    Serial.println(eepromByteRead(address));
  }
}
void loop(){}

void eepromByteWrite(unsigned int addr, byte byteToWrite){
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write((byte)(addr>>8));
  Wire.write((byte)(addr&0xFF));
  Wire.write(byteToWrite);
  Wire.endTransmission();
  delay(5); // important!
}

int eepromByteRead(unsigned int addr){
  int byteToRead;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write((byte)(addr>>8));
  Wire.write((byte)(addr&0xFF));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 1);
  byteToRead = Wire.read();
  return byteToRead;
}

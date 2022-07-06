//
//    FILE: I2C_small_eeprom_test.ino
//  AUTHOR: Tyler Freeman
// VERSION: 0.1.1
// PURPOSE: show/test I2C_EEPROM library with small EEPROMS
// HISTORY
// 0.1.0    2014-05-xx initial version
// 0.1.1    2020-07-14 fix #1 compile for ESP; fix author


#include <Wire.h>
#include <I2C_eeprom.h>
#define EEPROM_ADR 0x51

// the address of your EEPROM
#define DEVICEADDRESS (0x51)
#define EE24LC01MAXBYTES 1024/8
#define SERIAL_DEBUG Serial
byte TEST_ADDR =15;



I2C_eeprom eeprom(DEVICEADDRESS, EE24LC01MAXBYTES);
void readAndWriteVar() {
    SERIAL_DEBUG.println("----------------------------------------------");
    SERIAL_DEBUG.print("SINGLE BYTE: writing and retreiving EEPROM on I2C at address ");
    SERIAL_DEBUG.println(DEVICEADDRESS);
    SERIAL_DEBUG.println("----------------------------------------------");

    byte curval = eeprom.readByte(TEST_ADDR);
    for(int data=10;TEST_ADDR <250;TEST_ADDR +=5,data+=10){
        eeprom.writeByte(TEST_ADDR, data);}

    SERIAL_DEBUG.print("updating to: ");
    SERIAL_DEBUG.println(curval);
    delay(10);

    curval = eeprom.readByte(TEST_ADDR);
    SERIAL_DEBUG.print("new value: ");
    SERIAL_DEBUG.println(curval);
}

byte readEEPROM(long eeaddress)
{
  Wire.beginTransmission(EEPROM_ADR);
Wire.write(eeaddress);
//  Wire.write((int)(eeaddress >> 8)); // MSB
//  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ADR, 1);

  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void setup()
{
  SERIAL_DEBUG.begin(115200);
  while (!SERIAL_DEBUG); // wait for SERIAL_DEBUG port to connect. Needed for Leonardo only

  SERIAL_DEBUG.println("IT IS BEGINNING");
  SERIAL_DEBUG.println("WAIT FOR IT");

  eeprom.begin();

  readAndWriteVar();


  SERIAL_DEBUG.println("\nDone...");

  
  //Start the I2C Library
  Wire.begin();
  Wire.setClock(400000); 

//Start the serial port
  //Serial.begin(115200);

  //Output raw bytes to terminal
  //In this case we're going to read all of the bytes
  //which is 32000, or in hex, 0x7D00
  for (long x = 0 ; x < 100 ; x++) //Read all 131,071 bytes from EERPOM
  {
    byte val = readEEPROM(x);
    Serial.print(x);
    Serial.print('\t');
    Serial.println(val);
  }


  
}


void loop()
{
  // Nothing to do during loop
}

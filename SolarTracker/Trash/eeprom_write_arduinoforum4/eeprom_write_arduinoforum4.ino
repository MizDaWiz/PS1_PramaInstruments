#include <Wire.h>
//Christensen v3.0 extEEPROM.h
#include <extEEPROM.h>

//24C02 EEPROM 2kbits, 256 bytes, 16 byte page length
//constructor
//extEEPROM(eeprom_size_t deviceCapacity, byte nDevice, unsigned int pageSize, byte eepromAddr = 0x50);

extEEPROM eep(kbits_2, 1, 16, 0x50);

char test[] = {
  "Hello-1234567890-and-abcdefghijklmnopqrstuvwxyz-Goodbye\n"
};//null terminated character array

char buffer[100]; //output buffer

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  int length = sizeof(test);
  Serial.print("Length   ");
  Serial.print(length);
  Serial.println();

  //return byte z for error checking on write over end of EEPROM
  //EEPROM_ADDR_ERR defined as 9 in library

  byte error = eep.write (0, test, sizeof(test)); //included null terminator
  Serial.print("Error status returned = ");
  Serial.println(error);
  if (error == 9) {
    Serial.println("ERROR ADDRESS TOO LARGE FOR EEPROM");
    return;
  }
  if (error != 0 && error != 9) {
    Serial.println("I2C ERROR");
    return;
  }
  if (error == 0) {
    Serial.println("No Errors");
  }

  eep.read(0, buffer, sizeof(test));

  Serial.println(buffer);
  Serial.println();

}

void loop()
{
}

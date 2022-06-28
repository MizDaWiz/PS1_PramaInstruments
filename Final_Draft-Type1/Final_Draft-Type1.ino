#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
DS3232RTC myRTC;
#include <EEPROM.h>
#include "I2C_eeprom.h"
I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);


int address = 0;
byte value;

int LDRpin1 = 27;
int LDRpin2 = 26;
int ENB=9;
int al1=10;
int al2=11;
int reading1 = 0;
int reading2 = 0;
int diff = 0;
int mindiff=50;
int maxaddress = 72;

//
//void moveActuator(diff)
//{
//   if(abs(diff)>mindiff)
//  {
//  if (diff> 0)
//  {
//    digitalWrite(al1,HIGH);
//    digitalWrite(al2,LOW);
//  }
//  else
//  {
//    digitalWrite(al2,HIGH);
//    digitalWrite(al1,LOW);
//  }
//  }
// else
//  {
//     digitalWrite(al1,LOW);
//    digitalWrite(al2,LOW);
//    
//  }
//}
void readEEPROM()
{
    // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();

  // advance to the next address of the EEPROM
  address = address + 1;

  // there are only 512 bytes of EEPROM, from 0 to 511, so if we're
  // on address 512, wrap around to address 0
  if (address == 512) {
    address = 0;
  }

  delay(500);
}




//void updateEEPROM(address,diff)
//{
//  i2c_eeprom_write_page(0x57, 0, (byte *)diff, sizeof(diff));
//  EEPROM.update(address, diff); //EEPROM of Arduino Mega
//}
void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);
  Wire.begin(); // initialise the connection
    pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
     
    myRTC.begin();
    setSyncProvider(myRTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet)
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
        
}

void loop() {
  int hr=hour();
  int mn=minute();
  int sc=second();
  reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  diff=reading1-reading2;
  Serial.println(diff);
  if(mn %2 !=0 || mn %2 ==0)//********Change sc to mn and change it to 20******
  {
    if(sc%20==0)
    {
//      i2c_eeprom_write_page(0x50, 0, (byte *)diff, sizeof(diff));
//      EEPROM.update(address, diff); //EEPROM of Arduino Mega
      address+=1;
            Serial.print(address);
          Serial.print("\t");
           Serial.print(hr );
         Serial.print("\t");
          Serial.print(mn );
         Serial.print("\t");
          Serial.print(sc );
         Serial.print("\t");  
          Serial.print(diff);
          Serial.println();
          delay(2000);
    }
  }
}

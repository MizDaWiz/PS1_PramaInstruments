#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
//#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
DS3232RTC myRTC;
//#include <EEPROM.h>
//#include "Wire.h"
//#include "I2C_eeprom.h"
//I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);

int LDRpin1 = A0;
int LDRpin2 = A1;
int ENB=9;
int al1=10;
int al2=11;
int reading1 = 0;
int reading2 = 0;
int diff = 0;
int mindiff=50;
int address = 0;
int maxaddress = 72;

void digitalClockDisplay()
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year());
    Serial.println();
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}



void setup()
{
    Wire.begin(); // initialise the connection
    pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
     Serial.begin(115200);
    myRTC.begin();
    setSyncProvider(myRTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet)
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
    
}

void loop()
{
    
  //clock setup ends

  int hr=hour();
  int mn=minute();
  int sc=second();
  if(mn %2 !=0 || mn %2 ==0)//********Change sc to mn and change it to 20******
  {
   
  if(sc%20==0)
 {
  Serial.println("Starting recording...");
   for(int address=0;address <=maxaddress;address++)
   {
     reading1 = analogRead(LDRpin1);
      reading2 = analogRead(LDRpin2);
      diff=reading1-reading2;
////      i2c_eeprom_write_page(0x57, 0, (byte *)diff, sizeof(diff));
//    
//    //EEPROM.update(address, diff); //EEPROM of Arduino Mega
//    //delay(15);
    
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
  delay(200);

}}
}


//  }
// 
//    
//  
  if(abs(diff)>mindiff)
  {
  if (diff> 0)
  {
    digitalWrite(al1,HIGH);
    digitalWrite(al2,LOW);
  }
  else
  {
    digitalWrite(al2,HIGH);
    digitalWrite(al1,LOW);
  }
  }
 else
  {
     digitalWrite(al1,LOW);
    digitalWrite(al2,LOW);
    
  }  }
  
  //else
//  {
// 
//    }}
//   }
//

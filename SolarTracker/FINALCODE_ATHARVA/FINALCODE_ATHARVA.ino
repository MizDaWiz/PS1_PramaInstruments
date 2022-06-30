#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
DS3232RTC myRTC;
#include <wire.h>
#include <EEPROM.h>
#include "I2C_eeprom.h"
I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);

int pot= 28;
int potreadinglive=0;
int potreading=0;
 
int median=300;
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



// Function to read from EEPROM
//byte readEEPROM(int address, int i2c_address)
//{
//  // Define byte for received data
//  byte rcvData = 0xFF;
// 
//  // Begin transmission to I2C EEPROM
//  Wire.beginTransmission(i2c_address);
// 
//  // Send memory address as two 8-bit bytes
//  Wire.update((int)(address >> 8));   // MSB
//  Wire.update((int)(address & 0xFF)); // LSB
// 
//  // End the transmission
//  Wire.endTransmission();
// 
//  // Request one byte of data at current memory address
//  Wire.requestFrom(i2c_address, 1);
// 
//  // Read the data and assign to variable
//  rcvData =  Wire.read();
// 
//  // Return the data as function output
//  return rcvData;
//
//
//  delay(500);
//}


//void updateEEPROM(address,diff)
//{
//  i2c_eeprom_write_page(0x57, 0, (byte *)diff, sizeof(diff));
//  EEPROM.update(address, diff); 
//  Wire.beginTransmission(i2c_address);
// 
//  // Send memory address as two 8-bit bytes
//  Wire.update((int)(address >> 8));   // MSB
//  Wire.update((int)(address & 0xFF)); // LSB
// 
//  // Send data to be stored
//  Wire.update(potreadlive);
// 
//  // End the transmission
//  Wire.endTransmission();
// 
//  // Add 5ms delay for EEPROM
//  delay(5);
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
    pinMode(pot,INPUT);
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

 int potreading=analogRead(pot);
  int hr=hour();
  int mn=minute();
  int sc=second();
  reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  diff=reading1-reading2;
  Serial.println(diff);
  if(mn %2 !=0)//***Change sc to mn and change it to 20***
  {
    if(sc%20==0)
    {
     
// Print to Serial Monitor
  Serial.println("Start Recording...");
 
  // Run until maximum address is reached
 
  for (int address = 0; address <= maxaddress; address++) {
 
    
    potreadinglive = analogRead(pot);
 
    // Write to the servo
    // Delay to allow servo to settle in position
 int potreadinglive=analogRead(pot);
  Serial.println(potreadinglive);
  if (abs(potreadinglive-median)>10)
  {
    if ((potreadinglive-median)>0)
    {
      digitalWrite(al1,HIGH);
      digitalWrite(al2,LOW);
      Serial.println("up");
      
    }
    else if((potreadinglive-median)<0)
    {
      digitalWrite(al1,LOW);
      digitalWrite(al2,HIGH);
      Serial.println("down");
    }
  }
  else
  {
    digitalWrite(al1,LOW);
      digitalWrite(al2,LOW);
      Serial.println("nope");
    
      
    }
    delay(100);
    // Record the position in the external EEPROM
    //updateEEPROM(address , potreadinglive, EEPROM_I2C_ADDRESS);
 
    // Print to Serial Monitor
    Serial.print("ADDR = ");
    Serial.print(address);
    Serial.print("\t");
    Serial.print(hr );
    Serial.print("\t");
    Serial.print(mn );
    Serial.print("\t");
    Serial.print(sc );
    Serial.print("\t");  
    Serial.println(potreadinglive);
    Serial.println();
    delay(2000);
 
  }
 
  // Print to Serial Monitor
  Serial.println("Recording Finished!");
 
  // Delay 5 Seconds
  delay(5000);
 
  // Print to Serial Monitor
  Serial.println("Begin Playback...");
 
  // Run until maximum address is reached
 
  for (int address = 0; address <= maxaddress; address++) {
 
    // Read value from EEPROM
    //median = readEEPROM(address, EEPROM_I2C_ADDRESS);

    // Write to the servo
    // Delay to allow servo to settle in position
    // Convert value to integer for servo
    int potreadinglive=analogRead(pot);
  Serial.println(potreadinglive);
  if (abs(potreadinglive-median)>10)
  {
    if ((potreadinglive-median)>0)
    {
      digitalWrite(al1,HIGH);
      digitalWrite(al2,LOW);
      Serial.println("up");
      
    }
    else if((potreadinglive-median)<0)
    {
      digitalWrite(al1,LOW);
      digitalWrite(al2,HIGH);
      Serial.println("down");
    }
  }
  else
  {
    digitalWrite(al1,LOW);
      digitalWrite(al2,LOW);
      Serial.println("nope");
    
      
    }
    delay(100);
    // Print to Serial Monitor
    Serial.print("ADDR = ");
    Serial.print(address);
    Serial.print("\t");
    Serial.print(hr );
    Serial.print("\t");
    Serial.print(mn );
    Serial.print("\t");
    Serial.print(sc );
    Serial.print("\t");  
    Serial.println(median);
    Serial.println();
    delay(2000);
    
 
  }
 
  // Print to Serial Monitor
  Serial.println("Playback Finished!");
 
    }
  }
}

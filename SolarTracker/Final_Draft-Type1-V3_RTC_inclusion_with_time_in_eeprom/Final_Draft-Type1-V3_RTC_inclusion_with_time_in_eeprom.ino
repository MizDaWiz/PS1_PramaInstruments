#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
DS3232RTC myRTC;
#include <Wire.h>
#include <I2C_eeprom.h>
#define EEPROM_ADR 0x51

// the address of your EEPROM
#define DEVICEADDRESS (0x51)
#define EE24LC01MAXBYTES 1024/8

byte TEST_ADDR =0;
byte add;
int data;

I2C_eeprom eeprom(DEVICEADDRESS, EE24LC01MAXBYTES);

int pot=28;
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
//int maxaddress = 72;

void readAndWriteVar(long add, byte data) {

        eeprom.writeByte(add, data);

    Serial.print(add);
    Serial.print('\t');
    Serial.print(data);
    Serial.println('\t');
    delay(10);
}
byte readEEPROM(long eeaddress)
{
  Wire.beginTransmission(EEPROM_ADR);
  Wire.write(eeaddress);
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ADR, 1);

  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void setup() {
  
Serial.begin(115200);
delay(2000);
     pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    pinMode(pot,INPUT);
    digitalWrite(ENB,HIGH);  ///for Actuator

    

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

int hr=hour();
  int mn=minute();
  int sc=second();

    // SERIAL_DEBUG.begin(115200);
  while (!Serial); // wait for SERIAL_DEBUG port to connect. Needed for Leonardo only

  Serial.println("IT IS BEGINNING");
  Serial.println("WAIT FOR IT");

  eeprom.begin();
  for(;TEST_ADDR<80;TEST_ADDR+=4)
  {
  int potreading=(analogRead(pot))/4;
  readAndWriteVar(TEST_ADDR,potreading);
//  readAndWriteVar(TEST_ADDR+1,hr);
//  readAndWriteVar(TEST_ADDR+1,mn);
delay(2000);

  }
  Serial.println("\nDone...");
        
}

void loop() {
 //Start the I2C Library
  Wire.begin();
  Wire.setClock(400000);
  //reading eeprom
 for (long x = 0 ; x < 100 ; x++) //Read all 131,071 bytes from EERPOM
  {
    byte median = readEEPROM(x);
    Serial.print(median);
    Serial.print('\t');
    Serial.print(x);
    Serial.print('\t');
    
    int potreadinglive=(analogRead(pot))/4;
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
    
  }

  
// int potreading=analogRead(pot);
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

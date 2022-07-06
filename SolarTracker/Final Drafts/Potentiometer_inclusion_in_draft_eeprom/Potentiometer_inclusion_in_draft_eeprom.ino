int pot=28;
int ENB=9;
int al1=10;
int al2=11;

#include <Wire.h>
#include <I2C_eeprom.h>
#define EEPROM_ADR 0x51

// the address of your EEPROM
#define DEVICEADDRESS (0x51)
#define EE24LC01MAXBYTES 1024/8
#define SERIAL_DEBUG Serial
byte TEST_ADDR =0;
byte add;
int data;


I2C_eeprom eeprom(DEVICEADDRESS, EE24LC01MAXBYTES);
void readAndWriteVar(long add, byte data) {
//    SERIAL_DEBUG.println("----------------------------------------------");
//    SERIAL_DEBUG.print("SINGLE BYTE: writing and retreiving EEPROM on I2C at address ");
//    SERIAL_DEBUG.println(DEVICEADDRESS);
//    SERIAL_DEBUG.println("----------------------------------------------");

//    byte curval = eeprom.readByte(TEST_ADDR);
//    for(int data=10;TEST_ADDR <250;TEST_ADDR +=5,data+=10){
        eeprom.writeByte(add, data);
//        }

    SERIAL_DEBUG.print(add);
    SERIAL_DEBUG.print('\t');
    SERIAL_DEBUG.print(data);
    SERIAL_DEBUG.println('\t');
    
//    SERIAL_DEBUG.println(curval);
    delay(10);

//    curval = eeprom.readByte(TEST_ADDR);
//    SERIAL_DEBUG.print("new value: ");
//    SERIAL_DEBUG.println(curval);
    
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



void setup() {
  
  Serial.begin(115200);
     pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    pinMode(pot,INPUT);
    digitalWrite(ENB,HIGH);  ///for Actuator

    
    
// SERIAL_DEBUG.begin(115200);
  while (!SERIAL_DEBUG); // wait for SERIAL_DEBUG port to connect. Needed for Leonardo only

  SERIAL_DEBUG.println("IT IS BEGINNING");
  SERIAL_DEBUG.println("WAIT FOR IT");

  eeprom.begin();
  for(;TEST_ADDR<100;TEST_ADDR+=1)
  {
  int potreading=(analogRead(pot))/4;
  readAndWriteVar(TEST_ADDR,potreading);
delay(500);
  }
  SERIAL_DEBUG.println("\nDone...");


    
        
}

void loop()
{
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
  
  
    
  }

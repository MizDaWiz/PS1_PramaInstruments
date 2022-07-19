#include <DS3232RTC.h>
DS3232RTC myRTC;      //FOr RTC Module

#include <Wire.h> //For I2C Communication
#include <I2C_eeprom.h>   //For I2C And EEPROM compatibility

#define EEPROM_ADR 0x51   // the address of your EEPROM
#define EE24LC01MAXBYTES 1024
I2C_eeprom eeprom(EEPROM_ADR, EE24LC01MAXBYTES);    //Initialize EEPROM connection using I2C protocol

#include <MPU6050_light.h>
MPU6050 mpu(Wire);

byte addr = 25; //Start Address to write ON EEPROM
byte addr_Read = 25; //Start Address to read from EEPROM
byte max_addr = addr + 288; //288=24 hours* 3 reading per hour * 4 bytes per reading
//Given an arbitrary value so that we can keep it changing while testing to prevent overwriting same bit very frequently.

byte add;    //variable to store address to keep running in loop
int data;
int address = 0;
long delay_20min = 1200000;
long delay_20sec = 20000;
int hour_start = 5; //5 am
int hour_end = 20; //8 pm
int counter = 0; //Counter set to zero for calibration to be done once
byte value;
int comparator;

//Digital/Analog Pins Declaration
int ENB = 12; //For Enable Pin of Motor Driver
int al1 = 10; //For Motor Driver Input Pin1
int al2 = 11; //For Motor Driver Input Pin2
int LDRpin1 = 26;
int LDRpin2 = 27; //for LDRs
int pot = 28; //for potentiometer
int reading1 = 0;
int reading2 = 0; //FOr storing LDR readings
int diff = 0; //difference between the readings of both LDRs
int mindiff = 20; // This is the minimum accepted difference between the LDR readings
/*  Keep the mindiff value larger for less noise in the range of 25 to 35
  But keep it as small as possible for better precision    */
int LDR_calib()
{
  reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  diff = reading1 - reading2;
  Serial.println(diff);
  if (abs(diff) > mindiff)
  {
    if ((diff) > 0)
    {
      digitalWrite(al1, HIGH);
      digitalWrite(al2, LOW);
      Serial.println("up");
    }
    else if ((diff) < 0)
    {
      digitalWrite(al1, LOW);
      digitalWrite(al2, HIGH);
      Serial.println("down");
    }
  }
  else
  {
    digitalWrite(al1, LOW);
    digitalWrite(al2, LOW);
    Serial.println("No need to optimise");
    return 1;
  }
  return 0;
}
void calibMode() //Calibration Mode //Reads data and stores into EEPROM
{

  delay(3000);  //delay given to open Serial Monitor
  while (!Serial); // wait for Serial port to connect. Needed for Leonardo only
  Serial.println("Calibration Mode");
  Serial.println("Date and Time of Calibration:");
  Serial.print(day());
  Serial.print(",");
  Serial.print(month());
  Serial.print(",");
  Serial.print(year());
  Serial.println('\t');
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  if (LDR_calib() == 1)
  {
    eeprom.begin();
    while (addr < max_addr)
    {

      while (LDR_calib() == 1)
      {
        Serial.println("Reading \t");
        mpu.update();
        float z = mpu.getAngleZ();
        Serial.print("\tZ: ");
        Serial.println(mpu.getAngleZ());
        writeEEPROM(addr, hour());
        delay(500);
        writeEEPROM(addr + 1, minute());
        delay(500);
        writeEEPROM(addr + 2, potreading);
        addr += 4;
        delay(delay_20min); //Used while Actual Usage of Tracker
        //delay(delay_20sec);//Used For Testing on Scaled down time of 20 second rather than 20 minutes
      }
    }
    Serial.println("\nCalibration Done");
  }
}



void writeEEPROM(long add, byte data) //Write on EEPROM
{
  eeprom.writeByte(add, data);
  Serial.print(add);
  Serial.print('\t');
  Serial.print(data);
  Serial.println('\t');
  delay(10);
}
byte readEEPROM(long eeaddress) //Read From EEPROM
{
  Wire.beginTransmission(EEPROM_ADR);
  Wire.write(eeaddress);
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADR, 1);
  byte rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void printDigits(int digits) //for Clock Display Computation
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void setup()
{
  Serial.begin(115200);
  pinMode(ENB, OUTPUT);
  pinMode(al1, OUTPUT); // Configure pin 10 as an Output
  pinMode(al2, OUTPUT); // Configure pin 11 as an Output
  pinMode(pot, INPUT);
  digitalWrite(ENB, HIGH); ///for Actuator
  pinMode(LDRpin1, INPUT);
  pinMode(LDRpin2, INPUT);

  myRTC.begin();
  setSyncProvider(myRTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
    //MPU6050 setup
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("Done!\n");
}

void loop()
{
  if (counter == 0) //To Calibrate only Once to avoid Rewriting EEPROM multiple time so as to prevent EEPROM damage
  {
    calibMode();
    counter += 1;
  }
  else
  {
    Serial.println("Starting Daily Mode");
    Wire.begin();//Start the I2C Library
    Wire.setClock(400000);//40KHz frequency
    //reading eeprom
    for (long x = addr_Read ; x < max_addr ; x += 4) //Read 24 hr data
    {
      byte median = readEEPROM(x);
      comparator = (int)median;
      Serial.print(comparator);
      Serial.print('\t');
      Serial.print(x);
      Serial.print('\t');
      if (hour() < hour_start && hour() >= hour_end) //For Night MOde and For Homing Purposes
      {
        Serial.println("Night Mode");
        Serial.println("Homing Initiated");
        if (hour() == hour_end && minute() == 1)
        {
          if (second() < 8)
          {
            digitalWrite(al1, HIGH);
            digitalWrite(al2, LOW);
            Serial.println("Sleeping and Homing");
          }
          else
          {
            Serial.println("Sleeping");
          }
        }
      }
      else
      {
        mpu.update();
        float z = mpu.getAngleZ();
        Serial.print("\tZ: "); Serial.println(mpu.getAngleZ());
        if (abs(z - median) > mindiff)
        {
          if ((z - median) > 0)
          {
            digitalWrite(al1, HIGH);
            digitalWrite(al2, LOW);
            Serial.println("up");
          }
          else if ((z - median) < 0)
          {
            digitalWrite(al1, LOW);
            digitalWrite(al2, HIGH);
            Serial.println("down");
          }
        }
        else
        {
          digitalWrite(al1, LOW);
          digitalWrite(al2, LOW);
          Serial.println("No need to optimise");
        }
        delay(delay_20min);
      }

    }
  }
}

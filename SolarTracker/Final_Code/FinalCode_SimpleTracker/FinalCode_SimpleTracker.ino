#include <DS3232RTC.h>
DS3232RTC myRTC;      //FOr RTC Module

long delay_20min=1200000;
long delay_20sec=20000;

int hour_start=5; //5 am
int hour_end=20;  //8 pm

//Digital/Analog Pins Declaration
int ENB=9; //For Enable Pin of Motor Driver
int al1=10;//For Motor Driver Input Pin1
int al2=11;//For Motor Driver Input Pin2
int LDRpin1 = 27;
int LDRpin2 = 26; //for LDRs
int reading1 = 0;
int reading2 = 0; //FOr storing LDR readings
int diff = 0; //difference between the readings of both LDRs
int mindiff=10;// This is the minimum accepted difference between the LDR readings
            
     
void printDigits(int digits) //for Clock Display Computation
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void setup() 
{
  Serial.begin(115200);
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


void loop() 
{
 if(hour()<=hour_start && hour()>=hour_end)//For Night MOde and For Homing Purposes
  {
    Serial.println("Night Mode");
    Serial.println("Homing Initiated");
    if(hour()==hour_end && minute()==1)
    {
      if(second()<8)
      {
        digitalWrite(al1,HIGH);
        digitalWrite(al2,LOW);
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
    reading1 = analogRead(LDRpin1);
    reading2 = analogRead(LDRpin2);
    diff=reading1-reading2;
    Serial.println(diff);
    if (abs(diff)>mindiff)
      {
      if ((diff)>0)
          {
            digitalWrite(al1,HIGH);
            digitalWrite(al2,LOW);
            Serial.println("up");
          }
          else if((diff)<0)
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
          Serial.println("No need to optimise");
        }
        delay(delay_20min);
   }

}

     
  
                
      
   

  
    

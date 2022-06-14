#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
DS3232RTC myRTC;

int LDRpin1 = A0;
int LDRpin2 = A1;
int ENB=9;
int al1=10;
int al2=11;
int reading1 = 0;
int reading2 = 0;
int diff = 0;
int mindiff=50;

void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    printTime(t);
}

// print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}

// print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << monthShortStr(month(t)) << _DEC(year(t));
}

// Print an integer in "00" format (with leading zero),
// followed by a delimiter character to Serial.
// Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}



void setup()
{
    pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
    Serial.begin(115200);

     Serial << F( "\n" __FILE__ "\n" __DATE__ " " __TIME__ "\n" );
    myRTC.begin();//clock setup

    // setSyncProvider() causes the Time library to synchronize with the
    // external RTC by calling RTC.get() every five minutes by default.
    setSyncProvider(myRTC.get);
    Serial << F("RTC Sync");
    if (timeStatus() != timeSet) Serial << F(" FAIL!");
    Serial << endl;
}

void loop()
{
         //clock setup starts
    static time_t tLast;
    time_t t;
    tmElements_t tm;

    // check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
    if (Serial.available() >= 12) {
        // note that the tmElements_t Year member is an offset from 1970,
        // but the RTC wants the last two digits of the calendar year.
        // use the convenience macros from the Time Library to do the conversions.
        int y = Serial.parseInt();
        if (y >= 100 && y < 1000)
            Serial << F("Error: Year must be two digits or four digits!") << endl;
        else {
            if (y >= 1000)
                tm.Year = CalendarYrToTm(y);
            else    // (y < 100)
                tm.Year = y2kYearToTm(y);
            tm.Month = Serial.parseInt();
            tm.Day = Serial.parseInt();
            tm.Hour = Serial.parseInt();
            tm.Minute = Serial.parseInt();
            tm.Second = Serial.parseInt();
            t = makeTime(tm);
            myRTC.set(t);   // use the time_t value to ensure correct weekday is set
            setTime(t);
            Serial << F("RTC set to: ");
            printDateTime(t);
            Serial << endl;
            // dump any extraneous input
            while (Serial.available() > 0) Serial.read();
        }
    }

    t = now();
    if (t != tLast) {
        tLast = t;
        //printDateTime(t);
//        if (second(t) == 0) {
//            float c = myRTC.temperature() / 4.;
//            float f = c * 9. / 5. + 32.;
//            Serial << F("  ") << c << F(" C  ") << f << F(" F");
//        }
//        Serial << endl;
    }
  //clock setup ends

  int hr=hour();
  int mn=minute();
  int sc=second();
  if(mn %2 ==0)//***********************Change sc to mn and change it to 20*****************
  {
   // read thr value from the LDR sensor
  reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  // print the LDR sensor reading so you know its range
  diff=reading1-reading2;
  //printDateTime(t);
  //Serial.print(reading1 );
  //Serial.print("   " );
  //Serial.print(reading2);
  //Serial.print("    ");
  Serial.print(hr );
//  Serial.print(",");
  Serial.print(mn );
//  Serial.print(",");
  Serial.print(sc );
//  Serial.print(",");  
  Serial.print(diff);
 
 
    
  
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
  
  else
  {
//  if (t != tLast) {
//        tLast = t;
//        printDateTime(t);
//        if (second(t) == 0) {
//            float c = myRTC.temperature() / 4.;
//            float f = c * 9. / 5. + 32.;
//            Serial << F("  ") << c << F(" C  ") << f << F(" F");
//        }
//        Serial << endl;
//    }
   }

}

  

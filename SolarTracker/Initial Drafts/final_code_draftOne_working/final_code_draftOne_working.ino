// Set the date and time by entering the following on the Arduino
// serial monitor:
//  year,month,day,hour,minute,second,
//
// Where
//  year can be two or four digits,
//  month is 1-12,
//  day is 1-31,
//  hour is 0-23, and
//  minute and second are 0-59.
// Entering the final comma delimiter (after "second") will avoid a
// one-second timeout and will allow the RTC to be set more accurately.

#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming


DS3232RTC myRTC;

int LDRpin1 = A0;//for ldr //A0
int LDRpin2 = A1;//A1
float reading1 = 0;
float reading2 = 0;
float diff = 0;
int bright = 0;
void setup()
{
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
        printDateTime(t);
        if (second(t) == 0) {
            float c = myRTC.temperature() / 4.;
            float f = c * 9. / 5. + 32.;
            Serial << F("  ") << c << F(" C  ") << f << F(" F");
        }
        Serial << endl;
    }
  //clock setup ends
  int mn=minute();
  int sc=second();
  if(sc %5 ==0)//***********************Change sc to mn and change it to 20*****************
  {
    reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  // print the LDR sensor reading so you know its range
  diff=reading1-reading2;
      float data = diff;     //Serial Communication to python
      data = (data/1024.00) * 5;
      String dataToSend = String(data);
      Serial.println(dataToSend);
    delay(1025);
  }

//digitalClockDisplay();
    


}
//void ldr_diff(reading1,reading2)
//{//      // read thr value from the LDR sensor
////  reading1 = analogRead(LDRpin1);
////  reading2 = analogRead(LDRpin2);
////  // print the LDR sensor reading so you know its range
////  diff=reading1-reading2;
////      float data = diff;     //Serial Communication to python
////      data = (data/1024.00) * 5;
////      String dataToSend = String(data);
////      Serial.println(dataToSend);
//////      delay(1000);
//}
//void digitalClockDisplay()
//{
//    // digital clock display of the time
//    Serial.print(hour());
//    printDigits(minute());
//    printDigits(second());
//    Serial.print(' ');
//    Serial.print(day());
//    Serial.print(' ');
//    Serial.print(month());
//    Serial.print(' ');
//    Serial.print(year());
//    Serial.println();
//}
//
//void printDigits(int digits)
//{
//    // utility function for digital clock display: prints preceding colon and leading 0
//    Serial.print(':');
//    if(digits < 10)
//        Serial.print('0');
//    Serial.print(digits);
//}

// print date and time to Serial
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

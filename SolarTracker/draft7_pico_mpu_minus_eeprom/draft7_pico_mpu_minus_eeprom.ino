#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
DS3232RTC myRTC;
#include <EEPROM.h>
#include "Wire.h"
#include "I2C_eeprom.h"
I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC256);

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


//mpu variables
//const int i2c_addr = 0x3F;
// 
//int gyro_x, gyro_y, gyro_z;
//long gyro_x_cal, gyro_y_cal, gyro_z_cal;
//boolean set_gyro_angles;
//
//long acc_x, acc_y, acc_z, acc_total_vector;
//float angle_roll_acc, angle_pitch_acc;
//
//float angle_pitch, angle_roll;
//int angle_pitch_buffer, angle_roll_buffer;
//float angle_pitch_output, angle_roll_output;
//
//// Setup timers and temp variables
//long loop_timer;
//int temp;

//void setup_mpu_6050_registers(){
//
//  //Activate the MPU-6050
//  
//  //Start communicating with the MPU-6050
//  Wire.beginTransmission(0x68); 
//  //Send the requested starting register                                       
//  Wire.write(0x6B);  
//  //Set the requested starting register                                                  
//  Wire.write(0x00);
//  //End the transmission                                                    
//  Wire.endTransmission(); 
//                                              
//  //Configure the accelerometer (+/-8g)
//  
//  //Start communicating with the MPU-6050
//  Wire.beginTransmission(0x68); 
//  //Send the requested starting register                                       
//  Wire.write(0x1C);   
//  //Set the requested starting register                                                 
//  Wire.write(0x10); 
//  //End the transmission                                                   
//  Wire.endTransmission(); 
//                                              
//  //Configure the gyro (500dps full scale)
//  
//  //Start communicating with the MPU-6050
//  Wire.beginTransmission(0x68);
//  //Send the requested starting register                                        
//  Wire.write(0x1B);
//  //Set the requested starting register                                                    
//  Wire.write(0x08); 
//  //End the transmission                                                  
//  Wire.endTransmission(); 
//                                              
//}
//
//
//void read_mpu_6050_data(){ 
//
//  //Read the raw gyro and accelerometer data
//
//  //Start communicating with the MPU-6050                                          
//  Wire.beginTransmission(0x68);  
//  //Send the requested starting register                                      
//  Wire.write(0x3B);
//  //End the transmission                                                    
//  Wire.endTransmission(); 
//  //Request 14 bytes from the MPU-6050                                  
//  Wire.requestFrom(0x68,14);    
//  //Wait until all the bytes are received                                       
//  while(Wire.available() < 14);
//  
//  //Following statements left shift 8 bits, then bitwise OR.  
//  //Turns two 8-bit values into one 16-bit value                                       
//  acc_x = Wire.read()<<8|Wire.read();                                  
//  acc_y = Wire.read()<<8|Wire.read();                                  
//  acc_z = Wire.read()<<8|Wire.read();                                  
//  temp = Wire.read()<<8|Wire.read();                                   
//  gyro_x = Wire.read()<<8|Wire.read();                                 
//  gyro_y = Wire.read()<<8|Wire.read();                                 
//  gyro_z = Wire.read()<<8|Wire.read();     
////
////  Serial.print(acc_x);Serial.print('\t');
////  Serial.print(acc_y);Serial.print('\t');
////  Serial.print(acc_z);Serial.print('\t');
////  Serial.print(gyro_x);Serial.print('\t');
////  Serial.print(gyro_y);Serial.print('\t');
////  Serial.print(temp);Serial.print('\t');
//  
//  //delay(100);
//}
// print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << monthShortStr(month(t)) << _DEC(year(t));
}

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
    Wire.begin(0x68);//initialise the connection
    pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
    Serial.begin(115200);

//     Serial << F( "\n" _FILE_ "\n" _DATE_ " " _TIME_ "\n" );
    myRTC.begin();//clock setup

    // setSyncProvider() causes the Time library to synchronize with the
    // external RTC by calling RTC.get() every five minutes by default.
    setSyncProvider(myRTC.get);
    Serial << F("RTC Sync");
    if (timeStatus() != timeSet) Serial << F(" FAIL!");
    Serial << endl;




//     //Start I2C
////  Wire.begin(0x68);
////    
//  
//  //Setup the registers of the MPU-6050                                                       
//  setup_mpu_6050_registers(); 
//  
//  //Read the raw acc and gyro data from the MPU-6050 1000 times                                          
//  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  
//    read_mpu_6050_data(); 
//    //Add the gyro x offset to the gyro_x_cal variable                                            
//    gyro_x_cal += gyro_x;
//    //Add the gyro y offset to the gyro_y_cal variable                                              
//    gyro_y_cal += gyro_y; 
//    //Add the gyro z offset to the gyro_z_cal variable                                             
//    gyro_z_cal += gyro_z; 
//    //Delay 3us to have 250Hz for-loop                                             
//    delay(3);   
//}

//  // Divide all results by 1000 to get average offset
//  gyro_x_cal /= 1000;                                                 
//  gyro_y_cal /= 1000;                                                 
//  gyro_z_cal /= 1000;
//  
//  // Start Serial Monitor                                                 
//  Serial.begin(115200);
//  
//  // Init Timer 
//  loop_timer = micros();                                               
}

void loop()
{
//  // Get data from MPU-6050
//  read_mpu_6050_data();
//     
//  //Subtract the offset values from the raw gyro values
//  gyro_x -= gyro_x_cal;                                                
//  gyro_y -= gyro_y_cal;                                                
//  gyro_z -= gyro_z_cal;                                                
//         
//  //Gyro angle calculations . Note 0.0000611 = 1 / (250Hz x 65.5)
//  
//  //Calculate the traveled pitch angle and add this to the angle_pitch variable
//  angle_pitch += gyro_x * 0.0000611;  
//  //Calculate the traveled roll angle and add this to the angle_roll variable
//  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians                                
//  angle_roll += gyro_y * 0.0000611; 
//                                     
//  //If the IMU has yawed transfer the roll angle to the pitch angle
//  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);
//  //If the IMU has yawed transfer the pitch angle to the roll angle               
//  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               
//  
//  //Accelerometer angle calculations
//  
//  //Calculate the total accelerometer vector
//  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z)); 
//   
//  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
//  //Calculate the pitch angle
//  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296; 
//  //Calculate the roll angle      
//  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       
//  
//  //Accelerometer calibration value for pitch
//  angle_pitch_acc -= 0.0;
//  //Accelerometer calibration value for roll                                              
//  angle_roll_acc -= 0.0;                                               
//
//  if(set_gyro_angles){ 
//  
//  //If the IMU has been running 
//  //Correct the drift of the gyro pitch angle with the accelerometer pitch angle                      
//    angle_pitch = angle_pitch * 0.9 + angle_pitch_acc * 0.10; 
//    //Correct the drift of the gyro roll angle with the accelerometer roll angle    
//    angle_roll = angle_roll * 0.9+ angle_roll_acc * 0.1;        
//  }
//  else{ 
//    //IMU has just started  
//    //Set the gyro pitch angle equal to the accelerometer pitch angle                                                           
//    angle_pitch = angle_pitch_acc;
//    //Set the gyro roll angle equal to the accelerometer roll angle                                       
//    angle_roll = angle_roll_acc;
//    //Set the IMU started flag                                       
//    set_gyro_angles = true;                                            
//  }
//  
//
////To dampen the pitch and roll angles a complementary filter is used
//  //Take 90% of the output pitch value and add 10% of the raw pitch value
//  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1; 
//  //Take 90% of the output roll value and add 10% of the raw roll value 
//  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1; 
//  //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop  
//  
// //  Print to Serial Monitor   
//// Serial.print(" | Pitch Angle  = "); Serial.print(angle_pitch_output);
////  Serial.print("     | Roll Angle  = "); Serial.println(angle_roll_output);
////  delay(100);
//  
//// 
//// while(micros() - loop_timer < 4000); 
//// //Reset the loop timer                                
//// loop_timer = micros();










  
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
//  if(mn %2 !=0 || mn%2==0)//********Change sc to mn and change it to 20******
//  {
   
  if(sc%5==0)
 {
  Serial.println("Starting recording...");
   for(int address=0;address <=maxaddress;address++)
   {
      reading1 = analogRead(LDRpin1);
      reading2 = analogRead(LDRpin2);
      diff=reading1-reading2;
//      i2c_eeprom_write_page(0x57, 0, (byte *)diff, sizeof(diff));
    
    //EEPROM.update(address, diff); //EEPROM of Arduino Mega
    //delay(15);
    
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
  delay(5000);
// Serial.print(" | Pitch Angle  = "); Serial.print(angle_pitch_output);
//  Serial.print("     | Roll Angle  = "); Serial.println(angle_roll_output);
//  ///delay(100);
//  
// while(micros() - loop_timer < 4000); 
// //Reset the loop timer                                
// loop_timer = micros();
   }
  }
 
    
  
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
    
//  } 
  }
  
//  else
//  {
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
//  }

}

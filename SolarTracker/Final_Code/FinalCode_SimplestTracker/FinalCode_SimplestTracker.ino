long delay_20min=1200000;
long delay_20sec=20000;
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
            
void setup() 
{
  Serial.begin(115200);
  pinMode(ENB, OUTPUT);
  pinMode(al1, OUTPUT); // Configure pin 10 as an Output
  pinMode(al2, OUTPUT); // Configure pin 11 as an Output
  digitalWrite(ENB,HIGH);  ///for Actuator
  pinMode(LDRpin1, INPUT);
  pinMode(LDRpin2, INPUT);  
}


void loop() 
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


   

              
    
 


  

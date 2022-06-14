
int LDRpin1 = A0;
int LDRpin2 = A1;
int ENB=9;
int al1=10;
int al2=11;
int reading1 = 0;
int reading2 = 0;
int diff = 0;
int mindiff=50;
void setup()
{
    pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
    Serial.begin(115200);
}

void loop()
{
  // read thr value from the LDR sensor
  reading1 = analogRead(LDRpin1);
  reading2 = analogRead(LDRpin2);
  // print the LDR sensor reading so you know its range
  diff=reading1-reading2;
  Serial.print(reading1 );
  Serial.print("   " );
  Serial.print(reading2);
  Serial.print("    ");
  Serial.println(diff);
 
  if (abs(diff)>mindiff)
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
  }
}

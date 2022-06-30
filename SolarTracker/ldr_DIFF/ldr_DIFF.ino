int LEDpin = 9;
int LDRpin1 = A0;
int LDRpin2 = A;
int reading1 = 0;
int reading2 = 0;
int diff = 0;
int bright = 0;
void setup()
{
     pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);
  pinMode(LEDpin, OUTPUT);
  Serial.begin(9600);
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
  // map the LDR sensor reading to a range for the LED
  ////bright =reading/1023*255;
  //analogWrite(LEDpin, bright);
  //delay(100); // Wait for 100millisecond(s)
}

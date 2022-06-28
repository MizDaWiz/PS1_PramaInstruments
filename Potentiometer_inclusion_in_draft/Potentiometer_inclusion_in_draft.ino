int pot=xx;
int ENB=9;
int al1=10;
int al2=11;


void setup() {
  
  Serial.begin(115200);
     pinMode(ENB, OUTPUT);
    pinMode(al1, OUTPUT); // Configure pin 10 as an Output
    pinMode(al2, OUTPUT); // Configure pin 11 as an Output
    pinMode(pot,INPUT);
    digitalWrite(ENB,HIGH);  ///for Actuator
    pinMode(LDRpin1, INPUT);
    pinMode(LDRpin2, INPUT);        
}

void loop()
{
  int median=200;
  int potreadinglive=analogRead(pot);
  if (abs(potreadinglive-median)>10)
  {
    if ((potreadinglive-median)>0)
    {
      digitalWrite(al1,HIGH);
      digitalWrite(al2,LOW);
      
    }
    else if((potreadinglive-median)<0)
    {
      digitalWrite(al1,LOW);
      digitalWrite(al2,HIGH);
    }
  }
  else
  {
    digitalWrite(al1,LOW);
      digitalWrite(al2,LOW);
    
      
    }
    
  }

}

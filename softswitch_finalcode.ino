#define    SENSE 2       // Sense input to detect when power button is pressed. Pressed = HIGH
#define    DRIVE 7       // Controls the power FETS. HIGH = Circuit is powered. Write:LOW to turn off circuit.
#define HOLDTIME 500 // The length, in milliseconds, the user must hold the power button to power on the circuit.
#define HOLDTIME2 2000//The length, in milliseconds, the user must hold the power button to power off the circuit.
int control = 0;
void setup() {
  //All pins are input by default, so pinMode(SENSE,INPUT) is not necessary here
  pinMode(DRIVE,OUTPUT);
  digitalWrite(DRIVE,HIGH);
  
  //The LED in pin 13 will be our 'pseudo power-indicator'
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
}

void loop() {
  long unsigned int start;  // Used for timing
  long unsigned int now;    // Used for timing
 
  if(digitalRead(SENSE)){  // If power button pressed
    start = millis();      // store start time
    while(digitalRead(SENSE)){  //Loop while power button still held
      now = millis();            
      if(now > (start + HOLDTIME)){  //If button held for 1000ms
        digitalWrite(13,HIGH);    //Turn off power indicator
        while(digitalRead(SENSE)){} //Hold until power button is released
        delay(150);
        digitalWrite(DRIVE,HIGH); // Kill power to the whole circuit
      }
    while(digitalRead(SENSE)){  //Loop while power button still held
      now = millis();            
      if(now > (start + HOLDTIME2)){  //If button held for 1000ms
        digitalWrite(13,LOW);    //Turn off power indicator
        while(digitalRead(SENSE)){} //Hold until power button is released
        delay(150);
        digitalWrite(DRIVE,LOW); // Kill power to the whole circuit
      }
    }
  }
}}

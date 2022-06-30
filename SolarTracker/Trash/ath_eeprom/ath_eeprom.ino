#include <EEPROM.h>

/* the current address in the EEPROM (i.e. which byte we're going to write to next) */
int address = 0;
byte value;
int maxaddress = 50;

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
 Serial.println("Starting recording...");
 for(int address=0;address <=maxaddress;address++)
 {
   int val = analogRead(A0) / 4;
    EEPROM.update(address, val);
    delay(15);
    
    Serial.print(address);
  Serial.print("\t");
  Serial.print(val, DEC);
  Serial.println();
  delay(200);
  }
  Serial.println("Recording Finished!");
  delay(1000);
  Serial.println("Begin Playback");

  for(int address=0;address <=maxaddress; address++){
     value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
 
    }
Serial.println("Playback Finished");
  }


void loop() {  delay(500);
}

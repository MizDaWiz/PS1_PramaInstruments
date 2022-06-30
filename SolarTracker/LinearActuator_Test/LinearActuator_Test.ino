
const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup() {
 Serial.begin(9600);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
   digitalWrite(7,HIGH);
}

void loop() {

 //Check to see if anything is available in the serial receive buffer
 while (Serial.available() > 0)
 {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte == 'o')
   {
     digitalWrite(5, HIGH);
     digitalWrite(6, LOW);
  delay(1000);  
   }
   else if(inByte =='s')
   {
     digitalWrite(5, LOW);  
     digitalWrite(6, LOW);
  delay(1000);  
   }
   //Full message received...
   else if(inByte =='p')
   {
     digitalWrite(5, LOW);  
     digitalWrite(6, HIGH);
  delay(1000);  
   }
 }
}

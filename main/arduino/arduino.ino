
#include <SoftwareSerial.h>

SoftwareSerial espSerial(0, 1);//RX TX
// defines pins numbers
const int ledPin = 9;
const int irPin = 10;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(ledPin, OUTPUT);  
  pinMode(irPin,INPUT);  
//  Serial.begin(115200); // Starts the serial communication
  espSerial.begin(115200);
}
void loop() {
  
  if (digitalRead(ir)==LOW){ //object is detected  
    Serial.println("Led LOW"); 
    digitalWrite(led,LOW);  
    ledVal = 0;
  }  
  else{ //no object detected  
    Serial.println("Led HIGH"); 
    digitalWrite(led,HIGH);  
    ledVal = 1;
  }  

  // Prints the distance on the Serial Monitor
  espSerial.println(String("{\"cm\":")+String(distance)+String("}"));
  delay(50);
}
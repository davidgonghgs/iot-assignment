
#include <SoftwareSerial.h>

// SoftwareSerial espSerial(0, 1);//RX TX
// defines pins numbers
const int ir1 = 8;
const int led1 = 9;

const int ir2 = 5;
const int led2 = 6;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(ir1,INPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(ir2,INPUT);  
  pinMode(led2, OUTPUT);  
  Serial.begin(115200); // Starts the serial communication
  // espSerial.begin(115200);
}
void loop() {
  
  if (digitalRead(ir1)==LOW){ //object is detected  
    Serial.println("Led1 LOW"); 
    digitalWrite(led1,LOW);  
  }  
  else{ //no object detected  
    Serial.println("Led1 HIGH"); 
    digitalWrite(led1,HIGH);  
  }  

  if (digitalRead(ir2)==LOW){ //object is detected  
    Serial.println("Led2 LOW"); 
    digitalWrite(led2,LOW);  
  }  
  else{ //no object detected  
    Serial.println("Led2 HIGH"); 
    digitalWrite(led2,HIGH);  
  }  

  // Prints the distance on the Serial Monitor
  //espSerial.println(String("{\"cm\":")+String(distance)+String("}"));
  delay(100);
}
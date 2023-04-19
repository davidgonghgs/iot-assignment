const int ir1 = 8;
const int led1 = 9;

const int ir2 = 10;
const int led2 = 11;

//hc-04                                                                                                             
const int trigPin = 6; //trigPin D3 
const int echoPin = 5; //echoPin D1  
// defines variables
long duration;
int led1Val,led2Val;
int cm = 0;
void setup() {
  pinMode(ir1,INPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(ir2,INPUT);  
  pinMode(led2, OUTPUT);  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200);
}
void loop() {
  
  if (digitalRead(ir1)==LOW){ //object is detected  
    //Serial.println("Led1 LOW"); 
    digitalWrite(led1,LOW);  
    led1Val = 0;
  }  
  else{ //no object detected  
  //  Serial.println("Led1 HIGH"); 
    digitalWrite(led1,HIGH);  
    led1Val = 1;
  }  

  if (digitalRead(ir2)==LOW){ //object is detected  
   // Serial.println("Led2 LOW"); 
    digitalWrite(led2,LOW);  
    led2Val = 0;
  }  
  else{ //no object detected  
   // Serial.println("Led2 HIGH"); 
    digitalWrite(led2,HIGH);  
    led2Val = 1;
  }  
  hc04();
  char s[200];
  sprintf(s, "{\"led1\":%d,\"led2\":%d,\"cm\":%d}", led1Val,led2Val,cm);
  Serial.println(s);
  delay(1000);
}

void hc04(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  cm = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.println(String("{\"cm\":")+String(cm)+String("}"));
}
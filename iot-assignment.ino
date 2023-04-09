// static const uint8_t D0   = 16;
// static const uint8_t D1   = 5;
// static const uint8_t D2   = 4;
// static const uint8_t D3   = 0;
// static const uint8_t D4   = 2;
// static const uint8_t D5   = 14;
// static const uint8_t D6   = 12;
// static const uint8_t D7   = 13;
// static const uint8_t D8   = 15;
// static const uint8_t RX   = 3;
// static const uint8_t TX   = 1;

int led = 4; //LED D2 
int ir = 5; //ir sensor D1 
int buzzer = 13;// buzzer D7
const int trigPin = 14; // Trigger Pin of Ultrasonic Sensor  D5 
const int echoPin = 12; // Echo Pin of Ultrasonic Sensor D6 
long duration, cm, inches; 

void setup() {  
  pinMode(led, OUTPUT);  
  pinMode(ir,INPUT);  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(buzzer,OUTPUT); 
  Serial.begin(9600); 
}  

void loop() {  

  if (digitalRead(ir)==LOW){ //object is detected  
    Serial.println("IR LOW"); 
    digitalWrite(led,LOW);  
     delay(100);  
  }  
  else{ //no object detected  
    Serial.println("IR HIGH"); 
    digitalWrite(led,HIGH);  
    delay(100); 
  }  

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration * 0.034 / 2;
  Serial.print(cm); 
  Serial.println("cm");
  if(cm <= 30){
    tone(buzzer, 1000); // tone() is the main function to use with a buzzer, it takes 2 or 3 parameteres (buzzer pin, sound frequency, duration)
    delay(1000);
  }
  delay(250); 
}  




const int trigPin = 14; // Trigger Pin of Ultrasonic Sensor D6
const int echoPin = 12; // Echo Pin of Ultrasonic Sensor D5

long duration, cm, inches;

void setup() { 

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
} 
void loop() { 

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  duration = pulseIn(echoPin, HIGH);

  cm = (duration/2) / 29.1;
  Serial.print(duration);
  Serial.println("cm");

  delay(250);

} 


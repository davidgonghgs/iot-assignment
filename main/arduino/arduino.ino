const int ir1 = 8;
const int led1 = 9;

const int ir2 = 10;
const int led2 = 11;
// defines variables
long duration;
int distance,led1Val,led2Val;
void setup() {
  pinMode(ir1,INPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(ir2,INPUT);  
  pinMode(led2, OUTPUT);  
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
  char s[200];
  sprintf(s, "{\"led1\":%d,\"led2\":%d}", led1Val,led2Val);
  Serial.println(s);
  delay(20000);
}
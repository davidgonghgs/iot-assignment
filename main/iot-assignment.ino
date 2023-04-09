#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

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


const char* ssid = "Winnie"; //your WiFi Name
const char* password = "DavidGHS24";  //Your Wifi Password
WiFiServer server(80);

int led = 4; //LED D2 
int ir = 5; //ir sensor D1 
int buzzer = 13;// buzzer D7
const int trigPin = 14; // Trigger Pin of Ultrasonic Sensor  D5 
const int echoPin = 12; // Echo Pin of Ultrasonic Sensor D6 
long duration, cm, inches; 

void setup() {  
  Serial.begin(9600); 
  pinMode(led, OUTPUT);  
  pinMode(ir,INPUT);  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(buzzer,OUTPUT); 
  
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/"); 


}  

void loop() {  
 WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush(); 
  
  if (digitalRead(ir)==LOW){ //object is detected  
    Serial.println("IR LOW"); 
    digitalWrite(led,LOW);  
  }  
  else{ //no object detected  
    Serial.println("IR HIGH"); 
    digitalWrite(led,HIGH);  
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
  // if(cm <= 30){
  //   Serial.println("buzzer");    
  //   tone(buzzer,100,100); // tone() is the main function to use with a buzzer, it takes 2 or 3 parameteres (buzzer pin, sound frequency, duration)
  // }
  

  
  delay(500); 
}  



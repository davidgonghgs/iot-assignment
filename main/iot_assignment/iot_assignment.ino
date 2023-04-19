#include <ArduinoJson.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

//#include <SoftwareSerial.h>
#define SS_PIN 15  //D8
#define RST_PIN 16 //D0

// Define ThingSpeak credentials
unsigned long channelID =  2109747;
const char* writeAPIKey = "810Y1B680Q80M34N";


MFRC522 rfid(SS_PIN, RST_PIN); //实例化类
MFRC522::MIFARE_Key key;
//D8 SS
//D5 SCK
//D7 MOSI
//D6 MISO
//D0 RST

Servo servo;

// WiFi parameters to be configured
const char* ssid = "David"; // Write here your router's username
const char* password = "DavidGHS123"; // Write here your router's passward

//hc-04                                                                                                             
const int sg = 2; //sg90 2 D4           
                                                             
String UART_String="";
int angle = 0;
int led1 = 0;
int led2 = 0;
int cm = 0;
long duration;
int i; 
String rfidNumber="0";
// Init array that will store new NUID
byte nuidPICC[4];


 //using Wifi class
WiFiClient client;

void setup() {  

  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());


  //RFID
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  Serial.println();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println();
  Serial.println(F("This code scan the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

    

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
    // for sg90
  servo.attach(sg);
  servo.write(angle);
}  

void loop() {  
  rfidNumber = "0";
  rfidFunction();
  // hc04();

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(data);
   StaticJsonDocument<128> jsonDoc; // Create a JSON document
    DeserializationError error = deserializeJson(jsonDoc, data); // Deserialize the JSON string into the JSON document
    if (error) {
      Serial.print("Error deserializing JSON: ");
      Serial.println(error.c_str());
      return;
    }
    led1 = jsonDoc["led1"]; // Get the value of led1
    led2 = jsonDoc["led2"]; // Get the value of led2
    cm = jsonDoc["cm"]; // Get the value of led2
    Serial.print("led1 value: ");
    Serial.println(led1);
    Serial.print("led2 value: ");
    Serial.println(led2);
    Serial.print("cm value: ");
    Serial.println(cm);
  }

  //sg09
  // scan from 0 to 180 degrees
  for(angle = 10; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 



 // Send sensor value to ThingSpeak
  ThingSpeak.setField(1,led1);
  ThingSpeak.setField(2,cm);
  ThingSpeak.setField(3,rfidNumber);    
  ThingSpeak.setField(4,led2);    
  int status = ThingSpeak.writeFields(channelID,writeAPIKey);
  if (status == 200) {
    Serial.println("Sensor value sent to ThingSpeak!"); 
  }else {
    Serial.println("Problem updating channel. HTTP error code " + String(status));
  }
   delay(20000);
   Serial.println("----------------------------------------");    
}  

void rfidFunction(){
  //Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
      Serial.println(F("A new card has been detected."));

      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }

      Serial.println(F("The NUID tag is:"));
      Serial.print(F("In hex: "));

      Serial.print(hexToStr(rfid.uid.uidByte, rfid.uid.size));
      //    printHex(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
      Serial.print(F("In dec: "));
      rfidNumber = decToStr(rfid.uid.uidByte, rfid.uid.size);
      // printDec(rfid.uid.uidByte, rfid.uid.size);
      Serial.println(rfidNumber);

    }
    else 
    {
      Serial.println(F("The NUID tag is:"));
      Serial.print(F("In hex: "));
      Serial.print(hexToStr(rfid.uid.uidByte, rfid.uid.size));
      //printHex(rfid.uid.uidByte, rfid.uid.size);

      Serial.println();
      Serial.print(F("In dec: "));
      rfidNumber = decToStr(rfid.uid.uidByte, rfid.uid.size);
      Serial.println(rfidNumber);
      Serial.println(F("Card read previously."));
    }
    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

//convert hex to str
String hexToStr(byte *buffer, byte bufferSize) {
  String str = "";
  for (byte i = 0; i < bufferSize; i++) {
    str += String(buffer[i], HEX);
  }
  return str;
}


String decToStr(byte *buffer, byte bufferSize) {
  String str = "";
  for (byte i = 0; i < bufferSize; i++) {
    str += String(buffer[i], DEC);
  }
  return str;
}




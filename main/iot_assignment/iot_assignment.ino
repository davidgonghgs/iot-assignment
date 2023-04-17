#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#define SS_PIN 15  //D8
#define RST_PIN 16 //D0
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

// Init array that will store new NUID
byte nuidPICC[4];

const int led = 0; //LED D3 
const int ir = 5; //ir sensor D1  
const int sg = 2; //sg90 2
int angle = 0;
int i; 



void setup() {  
  Serial.begin(115200);



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

    // Connect to WiFi
  WiFi.begin(ssid, password);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  pinMode(led, OUTPUT);  
  pinMode(ir,INPUT);  
  
    // for sg90
  servo.attach(sg);
  servo.write(angle);


}  

void loop() {  

  
  rfidFunction();
  
  if (digitalRead(ir)==LOW){ //object is detected  
    Serial.println("IR LOW"); 
    digitalWrite(led,LOW);  
  }  
  else{ //no object detected  
    Serial.println("IR HIGH"); 
    digitalWrite(led,HIGH);  
  }  

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

  delay(1000); 
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
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

  }
  else 
  {
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    Serial.print(hexToStr(rfid.uid.uidByte, rfid.uid.size));
    //printHex(rfid.uid.uidByte, rfid.uid.size);

    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
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





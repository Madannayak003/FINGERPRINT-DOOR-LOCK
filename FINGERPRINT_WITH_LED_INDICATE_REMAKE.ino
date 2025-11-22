#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h> //Add servo librar

int getFingerprintIDez();

Servo servo1; //Define servo name / object

int buzzerPin=5;
int toneFrequency=15 ;

boolean started = false;

#define servoPin A3 //Define pin number to which servo motor is connected
#define durationTime 5000 //Define the time it remains in the open position of the door lock (miliseconds)
#define servoMin 180//Open position
#define servoMax 0// Closed position
#define espSignalPin 6

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int REDLED   = 8;
int GREENLED = 4;
int BUZZ     = 10;

void setup()  
{
  Serial.begin(9600);
  pinMode(REDLED,OUTPUT); 
  pinMode(buzzerPin,OUTPUT); 
  pinMode(GREENLED,OUTPUT); 
  pinMode(BUZZ,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(espSignalPin, OUTPUT);
  digitalWrite(espSignalPin, LOW);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  
  Serial.begin(9600);
  Serial.println("**HELLO♡MADAN**");
  
  servo1.attach(servoPin); //Define pin number of the servo
  servo1.write(servoMax); //The position of the servo at the start of the program

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
  
   while (1);
  }Serial.println("Waiting for valid  finger...");  
}

void loop() {                  // run over and over again


 getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  digitalWrite(REDLED,HIGH);// red led

}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detoct");    
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint feature");
  return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
        
  }
   
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
        Serial.println("Did not find a match");
  return p;
  } 
   else{
    Serial.println("Unknown error");
    return p;
  } 
     
  // found a match!
  
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
    
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
    
 for (int i = 0; i < 5; i++){
  digitalWrite(BUZZ,HIGH);
  delay(50);
  digitalWrite(BUZZ,LOW);
  delay(50);
  }
    
 {  
  digitalWrite(REDLED,LOW);
  delay(500);
        
Serial.println("》》FINGERPRINT VERIFIED");     
        
  digitalWrite(GREENLED,HIGH);
  delay(1000);
  digitalWrite(GREENLED,LOW);
  delay(1000);  
          
}
   Serial.print("》》OPEN WITH ID #");
   Serial.println(finger.fingerID);
   Serial.print(" with confidence of ");
   Serial.println(finger.confidence); 
   Serial.println("》》》DEVICE OPEN");
    
   digitalWrite(GREENLED,HIGH);
   delay(500);
  
 for (int i = 0; i < 5; i++) {
   tone(buzzerPin, toneFrequency);
   digitalWrite(7,HIGH);
   delay(500);
   noTone(buzzerPin);
   digitalWrite(7,LOW);
   delay(500);
} 
   servo1.write(servoMin); 
   delay(durationTime); 

  for (int i = 0; i < 5; i++) {
   digitalWrite(7,HIGH);
   tone(buzzerPin, toneFrequency);
   delay(200);
   digitalWrite(7,LOW);
   noTone(buzzerPin);
   delay(200);   
}
    
  servo1.write(servoMax);

 /* Notify ESP32-CAM to take a photo
  digitalWrite(espSignalPin, HIGH);
  delay(1000);
  digitalWrite(espSignalPin, LOW);      
 */
    
  Serial.println("《《《DEVICE LOCKED");
 
  digitalWrite(GREENLED,LOW);
   
 /* for(int i = 0; i < 2; i++){
    digitalWrite(7,HIGH);
    delay(1000);
    digitalWrite(7,LOW);
    delay(1000);
    }
 */
   // return finger.fingerID;      
}

#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(5, 6);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int lock = 13; //connect the door lock to pin 12
int lock_delay = 1500; //change the delay from here

#include <Servo.h> 
 
Servo servo_test;        //initialize a servo object for the connected servo  
                
int angle = 0; 

#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A3, A2, A1, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, 2, 3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String password = "1111";
String mypassword;

int redled = 8;
int greenled = 9;

int counter = 0;
int attempts = 0;
int max_attempts = 3;
//-------------------------------------------------------------------------
void setup()  
{
  Serial.begin(9600);
  pinMode(lock, OUTPUT);
  digitalWrite(lock, LOW);
  Serial.begin(9600);
  finger.begin(57600);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(lock, OUTPUT);
  servo_test.attach(11);      // attach the signal pin of servo to pin9 of arduino
  digitalWrite(redled, LOW);
  digitalWrite(greenled, LOW);
  digitalWrite(lock, LOW);
  Serial.println("enter password");
  servo_test.write(0);
  delay(1000);
  servo_test.write(90);
  delay(1000);
  servo_test.write(0);

  
}

void loop()
{
  getFingerprintIDez();
  delay(50);
  keypadfunction();
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
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
      Serial.println("Could not find fingerprint features");
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
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  digitalWrite(greenled, HIGH);
  servo_test.write(90);
  delay(5000);
  digitalWrite(greenled, LOW);
  servo_test.write(0);
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(greenled, HIGH);
  servo_test.write(90);
  delay(5000);
  digitalWrite(greenled, LOW);
  servo_test.write(0);
  return finger.fingerID;
}



void keypadfunction()
{
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    counter = counter + 1;
  }
  if (key == '1')
  {

    mypassword = mypassword + 1;
  }

  if (key == '2')
  {

    mypassword = mypassword + 2;
  }

  if (key == '3')
  {

    mypassword = mypassword + 3;
  }

  if (key == '4')
  {

    mypassword = mypassword + 4;
  }

  if (key == '5')
  {

    mypassword = mypassword + 5;
  }

  if (key == '6')
  {

    mypassword = mypassword + 6;
  }

  if (key == '7')
  {

    mypassword = mypassword + 7;
  }

  if (key == '8')
  {

    mypassword = mypassword + 8;
  }

  if (key == '9')
  {

    mypassword = mypassword + 9;
  }

  if (key == '0')
  {

    mypassword = mypassword + 0;
  }


  if (key == 'D')
  {
    Serial.println(mypassword);

    if ( password == mypassword )
    {
      Serial.println("Welcome To");
     digitalWrite(greenled, HIGH);
  servo_test.write(90);
  delay(5000);
  digitalWrite(greenled, LOW);
  servo_test.write(0);
      mypassword = "";
      counter = 0;
    }
    else
    {
      Serial.println("wrong");
      digitalWrite(redled, HIGH);
      delay(2000);
      digitalWrite(redled, LOW);
      attempts = attempts + 1;
      if (attempts >= max_attempts )
      {
        digitalWrite(redled, HIGH);
        delay(5000);
        digitalWrite(redled, LOW);
        attempts = 0;

      }
      mypassword = "";
      counter = 0;
      delay(100);
    }
  }
}
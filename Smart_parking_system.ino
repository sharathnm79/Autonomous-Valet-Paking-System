#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C_Hangul.h>
#include <ESP32Servo.h>
LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);
#include <IOXhop_FirebaseESP32.h>

#define SS_PIN 5
#define RST_PIN 2
const int buzzer = 13;
const int servoPin = 12;
const int IR1 = 14;
const int IR2 = 15;
const int IR3 = 27;
const int IR4 = 26;

String walletbalance2;
int updatedbal2;
String walletbalance1;
int updatedbal1;


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;

#define FIREBASE_HOST "valet-38e2a-default-rtdb.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "nL9PMsTw9ICX81ouieSRoQ7qIJZIFzeDlYE8MNg9"            // the secret key generated from firebase
#define WIFI_SSID "Suresh"                                           // input your home or public wifi name 
#define WIFI_PASSWORD "Sp@602528"

void setup()
{
  Serial.begin(115200);   // Initiate a serial communication
  delay(200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  delay(200);
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  delay(200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(200);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(buzzer, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  buzzeron();
  delay(4000);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME   ");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000);
  myservo.write(0);
  delay(2000);
}

void readrfid()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "95 50 0C AD") ) //change here the UID of the card/cards that you want to give access
  {
    buzzeron();
    Serial.println("Authorized access ");
    Serial.println();
    content = "";
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Authorized Access");
    delay(1000);
    walletbalance1 = Firebase.getString("walletbalance1");
    Serial.println(walletbalance1);
    updatedbal1 = walletbalance1.toInt() - 10;
    Serial.println(updatedbal1);
    Firebase.setString("walletbalance1", String(updatedbal1));
    if (walletbalance1.toInt() > 10)
    {
      delay(200);
      rotateservo90();

    }
  }

  else if ( (content.substring(1) == "99 45 0B 49") ) //change here the UID of the card/cards that you want to give access
  {
    buzzeron();
    Serial.println("Authorized access ");
    Serial.println();
    content = "";
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Authorized Access");
    delay(1000);
    walletbalance2 = Firebase.getString("walletbalance2");
    Serial.println(walletbalance2);
    updatedbal2 = walletbalance2.toInt() - 10;
    Firebase.setString("walletbalance2", String(updatedbal2));
    if (walletbalance2.toInt() > 10)
    {
      delay(200);
      rotateservo90();

    }
  }


  else   {
    buzzer1on();
    content = "";
    Serial.println("Access denied");
    lcd.setCursor(0, 1);
    lcd.print("Access Denied   ");
    delay(1000);

  }
  content = "";
}

void buzzeron()
{
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(200);
}

void buzzer1on()
{
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
  delay(200);
}

void rotateservo90()
{
  myservo.write(90);
  delay(2000);

}

void rotateservo0()
{
  myservo.write(0);
  delay(2000);
}

void checkcarout()
{
  if (digitalRead(IR1) == HIGH)
  {
    rotateservo0();
  }
}
void checkfreeslot()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VACANT: ");
  delay(500);
  boolean ir2val = digitalRead(IR2);
  if (ir2val == LOW)
  {
    Serial.println("Parking Slot 1 is FULL");
    Firebase.setString("Slot1", "FULL");
    delay(200);
    lcd.setCursor(9, 0);
    lcd.print(" ");
    delay(200);
  }

  else
  {
    Serial.println("Parking Slot 1 is VACANT");
    Firebase.setString("Slot1", "VACANT");
    delay(200);
    lcd.setCursor(9, 0);
    lcd.print("1");
    delay(200);
  }

  boolean ir3val = digitalRead(IR3);
  if (ir3val == LOW)
  {
    Serial.println("Parking Slot 2 is FULL");
    Firebase.setString("Slot2", "FULL");
    delay(200);
    lcd.setCursor(11, 0);
    lcd.print(" ");
    delay(200);
  }

  else
  {
    Serial.println("Parking Slot 2 is VACANT");
    Firebase.setString("Slot2", "VACANT");
    delay(200);
    lcd.setCursor(11, 0);
    lcd.print("2");
    delay(200);
  }


  boolean ir4val = digitalRead(IR4);
  if (ir4val == LOW)
  {
    Serial.println("Parking Slot 3 is FULL");
    Firebase.setString("Slot3", "FULL");
    delay(200);
    lcd.setCursor(13, 0);
    lcd.print(" ");
    delay(200);
  }

  else
  {
    Serial.println("Parking Slot 3 is VACANT");
    Firebase.setString("Slot3", "VACANT");
    delay(200);
    lcd.setCursor(13, 0);
    lcd.print("3");
    delay(200);
  }



}
void loop()
{
  readrfid();
  checkcarout();
  checkfreeslot();
}

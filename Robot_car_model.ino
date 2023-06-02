#include <IOXhop_FirebaseESP32.h>

#define FIREBASE_HOST "valet-38e2a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "nL9PMsTw9ICX81ouieSRoQ7qIJZIFzeDlYE8MNg9"
#define WIFI_SSID "Suresh"                                           // input your home or public wifi name 
#define WIFI_PASSWORD "Sp@602528"

const int motor1Pin1 = 13;
const int motor1Pin2 = 12;
const int motor2Pin1 = 14;
const int motor2Pin2 = 27;
const int echoPin = 18;
const int trigPin = 19;


String status1;
long duration;
int distance;


void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  delay(500);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);
  delay(200);
  //  forward();
  //  delay(2000);
  //  backward();
  //  delay(2000);
  //  left();
  //  delay(2000);
  //  right();
  //  delay(2000);

}


void readultrasonic()
{
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  delay(100);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("  cm");
  delay(200);
}

void forward() {
  int flag = 0;
  readultrasonic();
  while (distance > 20 && flag == 0)
  {

    Serial.println("Moving front");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    delay(1200);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(500);
    flag = 1;
  }
}

void backward() {

  Serial.println("Moving back");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(500);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(500);

}
void left() {

  Serial.println("Moving left");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(3000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(500);
}

void right() {

  Serial.println("Moving right");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(3250);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(500);
}



void loop() {

  status1 = Firebase.getString("STATUS");
  Serial.println(status1);
  delay(100);
  if (status1 == "1") {
    forward();

  }
  if (status1 == "2") {
    backward();

  }
  if (status1 == "4") {
    right();

  }
  if (status1 == "3") {
    left();

  }
  if (status1 == "7")
  {
    forward();
    forward();
    right();
    forward();

  }

  if (status1 == "6")
  {
    forward();
    forward();
    forward();
    forward();
    right();
    forward();

  }

  if (status1 == "8")
  {
    forward();
    forward();
    left();
    backward();
    forward();

  }

  if (status1 == "5") {
    Serial.println("Button Status Cleared");
    delay(500);

  }

}

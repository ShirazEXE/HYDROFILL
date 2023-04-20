#include <Servo.h>

int trigPin = 7;   //defines digital pin connected to trigger pin of ultrasonic sensor
int echoPin = 6;   //defines digital pin connected to echo pin of ultrasonic sensor
int pirPin = 2;    //defines digital pin connected to Passive-Infrared Sensor
int servoPin = 9;  //defines digital pin connected to Servo Motor
int valvePin = 12;
long unsigned int lowIn;
long unsigned int pause = 1000;
boolean lockLow = true;  // locklow is initialised to true
boolean takelowTime;
int PIRValue = 0;  // PIRValue is initialised to 0

Servo servo;  // defines an object of the Servo class

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);     //pirPin is set to take input
  pinMode(servoPin, OUTPUT);  //servoPin is set to provide output
  pinMode(trigPin, OUTPUT);   //trigPin is set to provide output
  pinMode(echoPin, INPUT);    //echoPin is set to take input
  servo.attach(servoPin);     //attaches servo pin to servo motor
  pinMode(valvePin, OUTPUT);

}

void PirSensor() {  //function to define the working PIR sensor

  if (digitalRead(pirPin) == HIGH)  //digitalRead command is used find whether pirPin is active or not and returns a boolean value
  {
    if (lockLow) {
      PIRValue = 1;
      lockLow = false;
      Serial.println("Motion Detected");
      servo.write(25);  // servo motor moves to 25 degree angle
      delay(1000);      // delay of 1 second
      servo.write(95);  // servo motor moves 95 degree
      delay(50);        // delay of 0.05 second
    }
    takelowTime = false;
  }
  if (digitalRead(pirPin) == LOW) {
    if (takelowTime) {
      lowIn = millis();
      takelowTime = false;
    }
    if (!lockLow && millis() - lowIn > pause) {
      PIRValue = 0;
      lockLow = true;
      Serial.println("Motion ended.");
    }
  }
}

void UltraSonic() {  //function to define the working Ultrasonic sensor

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/29.1)/2;  // Convert distance from microseconds to centimeters
  String p1 = " cm";
  Serial.println(distance + p1);
  if (distance < 13 && distance > 5) {  // Change this threshold value to adjust sensitivity
    digitalWrite(pirPin,LOW);
    Serial.println("Object detected!");
    digitalWrite(valvePin,LOW);
    Serial.println("Valve Opened!"); // RELAY ON
    delay(30000);
    digitalWrite(valvePin,HIGH);
    Serial.println("Valve Closed!");
    delay(3000); // RELAY OFF
  }
}

void loop() {  //Function to run a loop until the Arduino is turned off.

  PirSensor();   //Calls PIRSensor function.
  UltraSonic();  //Calls UltraSonic function
}

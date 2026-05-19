#include <ESP32Servo.h>

#define BUTTON 0
#define SERVO_PIN 4

Servo myServo;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);

  myServo.attach(SERVO_PIN);
  myServo.write(0); // start position
}

void loop() {
  int state = digitalRead(BUTTON);
  myServo.write(110);
  Serial.println("110 degree");
  delay(5000);
  if (state == HIGH) { // pressed
    Serial.println("BUTTON PRESSED");
  }
  myServo.write(0);
  Serial.println("0 degree"); 
  // } else {
  //   myServo.write(0);
  // }

  delay(50);
}
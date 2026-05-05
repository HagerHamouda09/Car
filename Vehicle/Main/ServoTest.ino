#include <ESP32Servo.h>

Servo myServo;

void setup() {
  myServo.attach(4); // signal pin
}

void loop() {

  myServo.write(180); // max
  delay(1000);
}

#include <Arduino.h>

#include <ESP32Servo.h>

#include "Servo.h"

Servo myServo;

void Servo_Init()
{
  myServo.attach(SERVO, 500, 2400);
  myServo.write(90);
}

void Set_Servo_Angle(int angle)
{
  myServo.write(angle); 

}


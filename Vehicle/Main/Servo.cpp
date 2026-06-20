#include <Arduino.h>
#include <ESP32Servo.h>
#include "Servo.h"

Servo myServo;

void Servo_Init()
{
    myServo.setPeriodHertz(50);           // force 50Hz — standard servo frequency, keeps it off motor timers
    myServo.attach(SERVO, 500, 2400);     // min/max pulse width in microseconds
    myServo.write(90);                    // neutral position
    delay(500);                           // give servo time to physically reach 90° before loop starts
}

void Set_Servo_Angle(int angle)
{
    myServo.write(angle);
}


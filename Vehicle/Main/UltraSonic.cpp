#include <Arduino.h>

#include "UltraSonic.h"

// speed unit cm/us
#define speed  0.0343 



void Ultrasonic_Init()
{
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);

  // extra making sure
  digitalWrite(TRIG,LOW);
}

float Get_Distance()
{
  long duration;
  float distance;

  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
    //trigger ultrasonic with a pulse to send ultrasonic waves
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  //measuring the time needed for the sensor to detect the reflected ultrasonic waves
  //duration in us
  //either to add timeout or to add an if condition to make sure not working o invalid data
  duration=pulseIn(ECHO,HIGH,30000);

  //distance in cm
  distance = (speed * duration)/2;
  return distance;
}

int Obstacle(){ //1 <=20 & 2<60 >20 & 3 safe
float distance= Get_Distance();
// Serial.println(distance);
  if(distance <= 20)
    return 1;
  else if(distance <= 60)
    return 2;
  else 
    return 3;

}
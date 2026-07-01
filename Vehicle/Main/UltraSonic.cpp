#include <Arduino.h>

#include "UltraSonic.h"

// speed unit cm/us
#define speed  0.0343 



void Ultrasonic_Init()
{
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);

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

  duration=pulseIn(ECHO,HIGH,30000);

  //distance in cm
  distance = (speed * duration)/2;
  return distance;
}

int Obstacle()
{ 
float distance= Get_Distance();
  if(distance <= 20)
    return 1;
  else if(distance <= 60)
    return 2;
  else 
    return 3;

}
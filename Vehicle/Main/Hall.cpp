#include <Arduino.h>

#include "Hall.h"

volatile long HallPulseCount=0;

unsigned long previousMillis=0;
unsigned long currentMillis=0;

const int Interval= 1000;

float RPM=0;

//ISR
void updateCount()
{
  HallPulseCount++;
}

void Hall_Init()
{
  pinMode(HALL,INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(HALL),
    updateCount,
    FALLING
  );
}

void Calc_Speed()
{
  currentMillis= millis();

  if(currentMillis- previousMillis >= Interval)
  {
  previousMillis= currentMillis;

  noInterrupts();
  long pulses = HallPulseCount;
  HallPulseCount=0;
  interrupts();

  RPM = pulses*(60000.0f/Interval);
  }
 
}

float Get_Speed()
{
  return RPM;
}



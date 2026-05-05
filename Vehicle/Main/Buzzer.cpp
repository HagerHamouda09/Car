//Buzzer
#include <Arduino.h>

#include "Buzzer.h"

void Buzzer_Init()
{
  pinMode(BUZZER,OUTPUT);
  Buzzer_Off();
}

void Buzzer_On()
{
  digitalWrite(BUZZER,HIGH);
}

void Buzzer_Off()
{
  digitalWrite(BUZZER,LOW);
}

void Set_Buzzer_Pattern(SYSTEM_CASES pattern);

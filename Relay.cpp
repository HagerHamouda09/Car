#include <Arduino.h>

#include "Relay.h"

void Relay_Init()
{
    pinMode(RELAY,OUTPUT);
    Relay_Off();
}

//signal to motor driver
void Relay_ON()
{
    digitalWrite(RELAY, HIGH);
}

// cut battery to stop motor driver 
void Relay_Off()
{
    digitalWrite(RELAY,LOW);
}

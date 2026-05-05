#include <Arduino.h>

#include "Button.h"

void Button_Init()
{
  pinMode(BUTTON,INPUT_PULLUP);
}

int Get_Button_State()
{
   return !digitalRead(BUTTON);
}


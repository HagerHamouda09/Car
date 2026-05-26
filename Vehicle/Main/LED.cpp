#include "LED.h"
#include "config.h"

static bool ledState = false;

// =========================================
void LED_Init()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
}

// =========================================
void LED_On()
{
    ledState = true;
    digitalWrite(LED, HIGH);
}

// =========================================
void LED_Off()
{
    ledState = false;
    digitalWrite(LED, LOW);
}

// =========================================
void LED_Toggle()
{
    ledState = !ledState;
    digitalWrite(LED, ledState);
}
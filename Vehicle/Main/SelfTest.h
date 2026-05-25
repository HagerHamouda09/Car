#ifndef SELFTEST_H
#define SELFTEST_H

#include <Arduino.h>

bool Run_Car_SelfTest();

bool MPU_Test();
bool Ultrasonic_Test();
bool Hall_Test();
bool BLE_Test();

#endif
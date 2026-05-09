#include <Arduino.h>
#include <Wire.h>
#include "MPU.h"

#define MPU_ADDRESS  0x68
#define PWR_MGMT_1   0x6B

float Acceleration = 0;
float Rotation_Speed = 0;

void MPU_init()
{
  Wire.begin(SDA, SCL);
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();
}

void Calc_MPU()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 14);

  int16_t ax = (Wire.read() << 8) | Wire.read();
  int16_t ay = (Wire.read() << 8) | Wire.read();
  int16_t az = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read(); // skip temp
  int16_t gx = (Wire.read() << 8) | Wire.read();
  int16_t gy = (Wire.read() << 8) | Wire.read();
  int16_t gz = (Wire.read() << 8) | Wire.read();

  Acceleration   = ax / 16384.0;
  Rotation_Speed = gz / 131.0;
  //16384,131 -> converting Raw numbers to real units->from datasheet
}

float Get_Acceleration()
{
  return Acceleration;
}

float Get_Rotation_Speed()
{
  return Rotation_Speed;
}

int Crash_Detect()
{
  static float prev_acc = 0;
  int crashed = 0;

  if(prev_acc > 0.5 && Acceleration < 0.5)
  {
    crashed = 1;
  }

  prev_acc = Acceleration;
  return crashed;
}

// int Get_Car_State(){


// }


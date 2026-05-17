#include <Arduino.h>
#include <Wire.h>
#include "MPU.h"

#define MPU_ADDRESS  0x68 //i2c addrees for mpu 
#define PWR_MGMT_1   0x6B //register address inside mpu

float Acceleration = 0;
float Rotation_Speed = 0; //degree / sec 

void MPU_init()
{
  Wire.begin(SDA, SCL);
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);// mpu start in sleep mode , writing 0x00 wakes it up
  Wire.endTransmission();
}

void Calc_MPU()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission(false); //Ends writing phase BUT keeps communication active.
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
  //default sensitivity
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
    Calc_MPU();
    float diff = abs(Acceleration - prev_acc);
    if (diff > 1.0)  // sudden change > 1g = crash
    {
        crashed = 1;
    }

    prev_acc = Acceleration;
    return crashed;
}
// int Get_Car_State(){


// }


// #include <Arduino.h>
// #include <Wire.h>
// #include "MPU.h"
// #include <math.h>

// #define MPU_ADDRESS  0x68
// #define PWR_MGMT_1   0x6B

// // =========================
// // GLOBAL VALUES
// // =========================
// float ax_g = 0;
// float ay_g = 0;
// float az_g = 0;

// float Rotation_Speed = 0;

// // =========================
// // INIT
// // =========================
// void MPU_init()
// {
//     Wire.begin(SDA, SCL);

//     Wire.beginTransmission(MPU_ADDRESS);
//     Wire.write(PWR_MGMT_1);
//     Wire.write(0x00);
//     Wire.endTransmission();

//     delay(100);
// }

// // =========================
// // READ MPU (CALL ONCE PER LOOP)
// // =========================
// void Calc_MPU()
// {
//     Wire.beginTransmission(MPU_ADDRESS);
//     Wire.write(0x3B);
//     Wire.endTransmission(false);

//     Wire.requestFrom(MPU_ADDRESS, 14);

//     int16_t ax = (Wire.read() << 8) | Wire.read();
//     int16_t ay = (Wire.read() << 8) | Wire.read();
//     int16_t az = (Wire.read() << 8) | Wire.read();

//     Wire.read(); Wire.read(); // temp skip

//     int16_t gx = (Wire.read() << 8) | Wire.read();
//     int16_t gy = (Wire.read() << 8) | Wire.read();
//     int16_t gz = (Wire.read() << 8) | Wire.read();

//     ax_g = ax / 16384.0;
//     ay_g = ay / 16384.0;
//     az_g = az / 16384.0;

//     Rotation_Speed = gz / 131.0;
// }

// // =========================
// // GETTERS
// // =========================
// float Get_Rotation_Speed() { return Rotation_Speed; }

// // =========================
// // CRASH DETECTION (SAFE)
// // =========================
// int Crash_Detect()
// {
//     static float prev_mag = 0;
//     static int spikeCount = 0;

//     float mag = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);

//     float diff = fabs(mag - prev_mag);

//     prev_mag = mag;

//     if (diff > 1.8) spikeCount++;
//     else spikeCount = 0;

//     if (spikeCount >= 2)
//     {
//         spikeCount = 0;
//         return 1;
//     }

//     return 0;
// }
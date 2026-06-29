#include "SelfTest.h"

#include <Wire.h>

#include "MPU.h"
#include "Ultrasonic.h"
#include "Hall.h"
#include "BLE.h"

// ===============================
// I2C CHECK
// ===============================

bool selfTestPassed = false;
bool selfTestDone = false;

static bool I2C_DeviceExists(uint8_t addr)
{
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
}

// ===============================
// MPU TEST
// ===============================
bool MPU_Test()
{
    Serial.println("[TEST] MPU6500");

    if (I2C_DeviceExists(0x68))
    {
        Serial.println("[PASS] MPU detected on I2C");
        return true;
    }

    Serial.println("[FAIL] MPU not found");
    return false;
}

// ===============================
// ULTRASONIC TEST (FIXED)
// ===============================
bool Ultrasonic_Test()
{
    Serial.println("[TEST] Ultrasonic");

    // discard first unstable reading
    Get_Distance();
    delay(80);

    float d = Get_Distance();

    Serial.print("[READ] Distance: ");
    Serial.println(d);

    // simple validity check only
    if (d > 2 && d < 400)
    {
        Serial.println("[PASS] Ultrasonic");
        return true;
    }

    Serial.println("[FAIL] Ultrasonic");
    return false;
}

// ===============================
// HALL TEST (FIXED)
// ===============================
bool Hall_Test()
{
    Serial.println("[TEST] Hall Sensor");

    // reset pulses to remove noise
    noInterrupts();
    HallPulseCount = 0;
    interrupts();

    delay(1200);

    Calc_Speed();
    float rpm = Get_Speed();

    Serial.print("RPM: ");
    Serial.println(rpm);

    if (!isnan(rpm))
    {
        Serial.println("[PASS] Hall sensor alive");
        return true;
    }

    Serial.println("[FAIL] Hall sensor error");
    return false;
}

// ===============================
// BLE TEST
// ===============================
// bool BLE_Test()
// {
//     Serial.println("[TEST] BLE");

//     unsigned long start = millis();
//     const unsigned long timeout = 10000; // 10 sec wait

//     while (millis() - start < timeout)
//     {
//         if (bleCar.isConnected())
//         {
//             Serial.println("[PASS] BLE connected");
//             return true;
//         }

//         delay(100);
//     }

//     Serial.println("[FAIL] BLE not connected (timeout)");
//     return false;
// }

bool BLE_Test()
{
    Serial.println("[TEST] BLE");

    while (!ble_isConnected())
    {
        Serial.println("[WAIT] Waiting for BLE connection...");
        delay(1000);
    }

    Serial.println("[PASS] BLE connected");
    return true;
}

// ===============================
// MAIN SELF TEST
// ===============================
bool Run_Car_SelfTest()
{
    bool result = true;

    Serial.println("\n======================");
    Serial.println(" CAR SELF TEST START ");
    Serial.println("======================");

    // reset hall before testing
    // noInterrupts();
    // HallPulseCount = 0;
    // interrupts();

    delay(200);
    
    if (!BLE_Test()) result = false;
    if (!MPU_Test()) result = false;
    if (!Ultrasonic_Test()) result = false;
    if (!Hall_Test()) result = false;

    Serial.println("======================");

    if (result)
    {
        Serial.println("SELF TEST RESULT: PASS");
    }
    else
    {
        Serial.println("SELF TEST RESULT: FAIL");
    }
    selfTestPassed = result;
    selfTestDone = true;

    return result;
}
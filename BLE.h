#ifndef BLE_CAR_H
#define BLE_CAR_H

#include <Arduino.h>
#include "config.h"

// -----------------------------------------------
// UUIDs  (Nordic UART Service style)
// RX = phone writes commands TO the car
// TX = car sends telemetry TO the phone
// -----------------------------------------------
#define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   // phone → ESP32
#define TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   // ESP32 → phone (notify)

// -----------------------------------------------
// Commands the phone can SEND to the car (1 byte)
// -----------------------------------------------
// 'E'  → trigger SYSTEM_EMERGENCY
// 'R'  → reset crash (go back to SYSTEM_IDLE)
// 'N'  → force SYSTEM_NORMAL (resume)
// 'I'  → SYSTEM_IDLE (pause everything)
// -----------------------------------------------

class BLECar {
public:
    BLECar();

    void begin();

    // Call this every loop — sends telemetry if connected
    void sendTelemetry(SYSTEM_CASES state, float speed, float distance);

    bool isConnected();

private:
    bool deviceConnected;

    static void handleCommand(char cmd);

    friend class CarServerCallbacks;
    friend class CarCharacteristicCallbacks;
};

// Single global instance — accessible from main
extern BLECar bleCar;

// Shared system state pointer so BLE callbacks can change it
extern SYSTEM_CASES systemState;
extern bool motorLocked;

#endif
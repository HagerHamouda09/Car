#ifndef BLE_CAR_H
#define BLE_CAR_H

#include <Arduino.h>
#include "config.h"

#define SIMULATION_BLE_ENABLED 1

#define SERVICE_UUID   "12345678-1234-1234-1234-1234567890AB"
#define RX_CHAR_UUID   "12345678-1234-1234-1234-1234567890AC"
#define TX_CHAR_UUID   "12345678-1234-1234-1234-1234567890AD"

#if SIMULATION_BLE_ENABLED
#define SIM_SERVICE_UUID "12345678-AAAA-BBBB-CCCC-123456789000"
#define SIM_RX_UUID      "12345678-AAAA-BBBB-CCCC-123456789001"
#define SIM_TX_UUID      "12345678-AAAA-BBBB-CCCC-123456789002"
#endif

class BLECar {

public:
    BLECar();

    void begin();
    void sendTelemetry(SYSTEM_CASES state);
    void handleReconnect();
    bool isConnected();

    static void handleCommand(char cmd);

#if SIMULATION_BLE_ENABLED
    static void handleSimCommand(char cmd);
#endif

private:
    bool deviceConnected;
};

extern BLECar bleCar;

extern SYSTEM_CASES systemState;
extern bool selfTestDone;
extern bool selfTestPassed;
extern int Trials;
extern bool motorLocked;

#endif
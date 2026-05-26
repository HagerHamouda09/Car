// #ifndef BLE_CAR_H
// #define BLE_CAR_H

// #include <Arduino.h>
// #include "config.h"

// // -----------------------------------------------
// // UUIDs  (Nordic UART Service style)
// // RX = phone writes commands TO the car
// // TX = car sends telemetry TO the phone
// // -----------------------------------------------
// #define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// #define RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   // phone → ESP32
// #define TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   // ESP32 → phone (notify)


// class BLECar {
// public:
//     BLECar();

//     void begin();

//     // Call this every loop — sends telemetry if connected
//     // void sendTelemetry(SYSTEM_CASES state, float speed, float distance);
//     void sendTelemetry(SYSTEM_CASES state);

//     bool isConnected();
//     void handleReconnect();

// private:
//     bool deviceConnected;

//     static void handleCommand(char cmd);

//     friend class CarServerCallbacks;
//     friend class CarCharacteristicCallbacks;
// };

// // Single global instance — accessible from main
// extern BLECar bleCar;

// // Shared system state pointer so BLE callbacks can change it
// extern SYSTEM_CASES systemState;
// extern bool motorLocked;

// #endif





// // This new code will be tested when integration with the mobile app to handle reconnection and automatic connection

// #ifndef BLE_CAR_H
// #define BLE_CAR_H

// #include <Arduino.h>
// #include "config.h"

// // ======================================================
// // UUIDs (Nordic UART Service Style)
// // ======================================================
// #define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

// #define RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // Phone -> ESP32
// #define TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // ESP32 -> Phone

// // ======================================================
// // BLE CLASS
// // ======================================================
// class BLECar {

// public:

//     BLECar();

//     void begin();

//     // Send telemetry to phone
//     void sendTelemetry(SYSTEM_CASES state);

//     // Handle auto reconnection
//     void handleReconnect();

//     // Check connection state
//     bool isConnected();

// private:

//     bool deviceConnected;

//     static void handleCommand(char cmd);

//     friend class CarServerCallbacks;
//     friend class CarCharacteristicCallbacks;
// };

// // ======================================================
// // GLOBAL OBJECT
// // ======================================================
// extern BLECar bleCar;

// // ======================================================
// // EXTERNAL SYSTEM VARIABLES
// // ======================================================
// extern SYSTEM_CASES systemState;

// extern bool selfTestDone;
// extern bool selfTestPassed;

// extern int Trials;

// #endif



// // // the new code with 2 services on one phone
// #ifndef BLE_CAR_H
// #define BLE_CAR_H

// #include <Arduino.h>
// #include "config.h"

// // ======================================================
// // SIMULATION ENABLE
// // ======================================================
// #define SIMULATION_BLE_ENABLED 1

// // ======================================================
// // DRIVER SERVICE UUIDS (UNCHANGED)
// // ======================================================
// #define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// #define RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
// #define TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// // ======================================================
// // SIMULATION SERVICE UUIDS (ADDED)
// // ======================================================
// #if SIMULATION_BLE_ENABLED

// #define SIM_SERVICE_UUID "12345678-AAAA-BBBB-CCCC-123456789000"
// #define SIM_RX_UUID      "12345678-AAAA-BBBB-CCCC-123456789001"
// #define SIM_TX_UUID      "12345678-AAAA-BBBB-CCCC-123456789002"

// #endif

// // ======================================================
// // CLASS
// // ======================================================
// class BLECar {

// public:
//     BLECar();

//     void begin();
//     void sendTelemetry(SYSTEM_CASES state);
//     void handleReconnect();
//     bool isConnected();

// private:

//     bool deviceConnected;

//     static void handleCommand(char cmd);

// #if SIMULATION_BLE_ENABLED
//     static void handleSimCommand(char cmd);
// #endif

//     friend class CarServerCallbacks;
//     friend class CarCharacteristicCallbacks;

// #if SIMULATION_BLE_ENABLED
//     friend class SimCallbacks;
// #endif
// };

// // ======================================================
// extern BLECar bleCar;

// extern SYSTEM_CASES systemState;
// extern bool selfTestDone;
// extern bool selfTestPassed;
// extern int Trials;
// extern bool motorLocked;

// #endif

#ifndef BLE_CAR_H
#define BLE_CAR_H

#include <Arduino.h>
#include "config.h"

#define SIMULATION_BLE_ENABLED 1

#define SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

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
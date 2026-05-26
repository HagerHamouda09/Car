// BLE.cpp

// #include "BLE.h"

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
// #include "Motor.h"

// extern int Trials;
// // --------------------------------------------------
// // Global BLE pointers
// // --------------------------------------------------
// static BLECharacteristic *pRxCharacteristic = nullptr;
// static BLECharacteristic *pTxCharacteristic = nullptr;
// static BLECar *instancePtr = nullptr;

// // --------------------------------------------------
// #define TELEMETRY_INTERVAL_MS 500

// // --------------------------------------------------
// // CONNECTION CALLBACKS
// // --------------------------------------------------
// class CarServerCallbacks : public BLEServerCallbacks {

//     void onConnect(BLEServer* pServer) override {
//         if (instancePtr) {
//             instancePtr->deviceConnected = true;
//         }

//         Serial.println("[BLE] Phone connected");
//     }

//     void onDisconnect(BLEServer* pServer) override {
//         if (instancePtr) {
//             instancePtr->deviceConnected = false;
//         }

//         Serial.println("[BLE] Phone disconnected");

//         BLEDevice::startAdvertising();
//     }

// //     void BLECar::handleReconnect() {
// //     if (!deviceConnected) {
// //         delay(500);
// //         BLEDevice::startAdvertising();
// //         Serial.println("[BLE] Restarting advertising...");
// //     }
// // }
// };

// // --------------------------------------------------
// // RX CALLBACK
// // --------------------------------------------------
// class CarCharacteristicCallbacks : public BLECharacteristicCallbacks {

//     void onWrite(BLECharacteristic *characteristic) override {

//         // ESP32 BLE library returns Arduino String
//         String value = characteristic->getValue();

//         if (value.length() > 0) {

//             char cmd = value[0];

//             Serial.print("[BLE] RX Command: ");
//             Serial.println(cmd);

//             BLECar::handleCommand(cmd);
//         }
//     }
// };

// // --------------------------------------------------
// // CONSTRUCTOR
// // --------------------------------------------------
// BLECar::BLECar() {
//     deviceConnected = false;
//     instancePtr = this;
// }

// // --------------------------------------------------
// // BLE INIT
// // --------------------------------------------------
// void BLECar::begin() {

//     BLEDevice::init("ESP32_CAR");

//     BLEServer *server = BLEDevice::createServer();
//     server->setCallbacks(new CarServerCallbacks());

//     BLEService *service = server->createService(SERVICE_UUID);

//     // ---------------- RX CHARACTERISTIC ----------------
//     // Phone writes commands here
//     pRxCharacteristic = service->createCharacteristic(
//         RX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_WRITE_NR
//     );

//     pRxCharacteristic->setCallbacks(
//         new CarCharacteristicCallbacks()
//     );

//     // ---------------- TX CHARACTERISTIC ----------------
//     // ESP32 sends telemetry here
//     pTxCharacteristic = service->createCharacteristic(
//         TX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_NOTIFY
//     );

//     pTxCharacteristic->addDescriptor(new BLE2902());

//     // --------------------------------------------------
//     service->start();

//     BLEAdvertising *advertising = BLEDevice::getAdvertising();

//     advertising->addServiceUUID(SERVICE_UUID);
//     advertising->setScanResponse(true);
//     advertising->start();

//     Serial.println("[BLE] BLE Started");
//     Serial.println("[BLE] Device Name: ESP32_CAR");
// }

// // --------------------------------------------------
// // SEND TELEMETRY
// // --------------------------------------------------
// void BLECar::sendTelemetry(
//     SYSTEM_CASES state
//     // float speed,
//     // float distance
// ) {

//     if (!deviceConnected) {
//         return;
//     }

//     static unsigned long lastSend = 0;

//     unsigned long now = millis();

//     if (now - lastSend < TELEMETRY_INTERVAL_MS) {
//         return;
//     }

//     lastSend = now;

//     // --------------------------------------------------
//     // Convert enum state to string
//     // --------------------------------------------------
//     const char* stateStr;

//     switch (state) {

//         case SYSTEM_CHECK:
//             stateStr = "CHECK";
//             break;
//         case SYSTEM_IDLE:
//             stateStr = "IDLE";
//             break;

//         case SYSTEM_READY:
//             stateStr = "READY";
//             break;

//         case SYSTEM_NORMAL:
//             stateStr = "NORMAL";
//             break;

//         case SYSTEM_CRASH:
//             stateStr = "CRASH";
//             break;

//         case SYSTEM_EMERGENCY:
//             stateStr = "EMERGENCY";
//             break;

//         case SYSTEM_OBSTACLESTOP:
//             stateStr = "OBSTACLE";
//             break;

//         case SYSTEM_SAFESTOP:
//             stateStr = "SAFESTOP";
//             break;
        
//         case SYSTEM_EXCEEDED_TRIALS:
//             stateStr = "TRIALS_EXCEEDED";
//             break;

//         case SYSTEM_CHECK_FAIL:
//             stateStr = "CHECK_FAIL";
//             break;

//         default:
//             stateStr = "UNKNOWN";
//             break;
//     }

//     // --------------------------------------------------
//     // Build telemetry packet
//     // --------------------------------------------------
//     char buffer[64];

//     snprintf(
//         buffer,
//         sizeof(buffer),
//         // "STATE:%s,SPD:%.1f,DIST:%.1f\n",
//         "STATE:%s\n",

//         stateStr
//         // speed,
//         // distance
//     );

//     // --------------------------------------------------
//     // Notify phone
//     // --------------------------------------------------
//     pTxCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
//     pTxCharacteristic->notify();

//     Serial.print("[BLE] TX -> ");
//     Serial.println(buffer);
// }

// // --------------------------------------------------
// // HANDLE COMMANDS
// // --------------------------------------------------
// void BLECar::handleCommand(char cmd) {

//     switch (cmd) {

//         // ----------------------------------------------
//         // EMERGENCY
//         // ----------------------------------------------
//         case 'P':

//             if (systemState != SYSTEM_IDLE)
//                 {
//                     Serial.println("[BLE] P ignored (not in IDLE)");
//                     return;
//                 }

//             if (!selfTestDone)
//             {
//                 Serial.println("[BLE] SELF TEST NOT DONE YET");
//                 return;
//             }

//             if (!selfTestPassed)
//             {
//                 Serial.println("[BLE] SELF TEST FAILED → IGNORING P");
//                 return;
//             }

//             systemState = SYSTEM_READY;
//             Serial.println("[BLE] SYSTEM_READY");

//             break;
            
//         case 'F':

//             Trials++;
//             if(Trials>=MaxTrials)
//                 {
//                     systemState = SYSTEM_EXCEEDED_TRIALS;
//                 }
//             break;
        
//         case 'E':

//             systemState = SYSTEM_EMERGENCY;

//             Serial.println("[BLE] SYSTEM_EMERGENCY");

//             break;
        

//         case 'N':

//         if (systemState != SYSTEM_READY)
//         {
//             Serial.println("[BLE] N ignored (not in READY)");
//             return;
//         }

//         Set_Motor_Speed(250);

//         Serial.println("[BLE] Motor start command");

//         break;


//         // // ----------------------------------------------
//         // // RESET AFTER CRASH
//         // // ----------------------------------------------
//         // case 'R':

//         //     // motorLocked = false;
//         //     systemState = SYSTEM_CRASH;

//         //     Serial.println("[BLE] RESET -> SYSTEM_CRASH");

//         //     break;

//         // // ----------------------------------------------
//         // // RESUME NORMAL
//         // // ----------------------------------------------
//         // case 'N':

//         //     // motorLocked = false;
//         //     systemState = SYSTEM_NORMAL;

//         //     Serial.println("[BLE] SYSTEM_NORMAL");

//         //     break;

//         // ----------------------------------------------
//         // // IDLE / STOP
//         // // ----------------------------------------------
//         // case 'I':

//         //     // motorLocked = true;
//         //     systemState = SYSTEM_IDLE;

//         //     Serial.println("[BLE] SYSTEM_IDLE");

//         //     break;

//         // // ----------------------------------------------
//         default:

//             Serial.println("[BLE] Unknown command");

//             break;
//     }
// }

// // --------------------------------------------------
// // CONNECTION STATUS
// // --------------------------------------------------
// bool BLECar::isConnected() {
//     return deviceConnected;
// }

// // --------------------------------------------------
// // GLOBAL OBJECT
// // --------------------------------------------------
// BLECar bleCar;



// // // This new code will be tested when integration with the mobile app to handle reconnection and automatic connection

// #include "BLE.h"

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

// #include "Motor.h"

// // ======================================================
// // GLOBAL POINTERS
// // ======================================================
// static BLECharacteristic *pRxCharacteristic = nullptr;
// static BLECharacteristic *pTxCharacteristic = nullptr;

// static BLECar *instancePtr = nullptr;

// // ======================================================
// #define TELEMETRY_INTERVAL_MS 500

// // ======================================================
// // CONNECTION CALLBACKS
// // ======================================================
// class CarServerCallbacks : public BLEServerCallbacks {

//     void onConnect(BLEServer* pServer) override {

//         if (instancePtr) {
//             instancePtr->deviceConnected = true;
//         }

//         Serial.println("[BLE] Phone connected");
//     }

//     void onDisconnect(BLEServer* pServer) override {

//         if (instancePtr) {
//             instancePtr->deviceConnected = false;
//         }

//         Serial.println("[BLE] Phone disconnected");

//         delay(200);

//         pServer->startAdvertising();

//         Serial.println("[BLE] Advertising restarted");
//     }
// };

// // ======================================================
// // RX CALLBACK
// // ======================================================
// class CarCharacteristicCallbacks : public BLECharacteristicCallbacks {

//     void onWrite(BLECharacteristic *characteristic) override {

//         String value = characteristic->getValue();

//         if (value.length() > 0) {

//             char cmd = value[0];

//             Serial.print("[BLE] RX Command: ");
//             Serial.println(cmd);

//             BLECar::handleCommand(cmd);
//         }
//     }
// };

// // ======================================================
// // CONSTRUCTOR
// // ======================================================
// BLECar::BLECar() {

//     deviceConnected = false;

//     instancePtr = this;
// }

// // ======================================================
// // BLE INIT
// // ======================================================
// void BLECar::begin() {

//     // --------------------------------------------------
//     // BLE INIT
//     // --------------------------------------------------
//     BLEDevice::init("ESP32_CAR");

//     // Strongest BLE power
//     BLEDevice::setPower(ESP_PWR_LVL_P9);

//     // Better BLE packet size
//     BLEDevice::setMTU(517);

//     // --------------------------------------------------
//     // CREATE SERVER
//     // --------------------------------------------------
//     BLEServer *server = BLEDevice::createServer();

//     server->setCallbacks(new CarServerCallbacks());

//     // --------------------------------------------------
//     // CREATE SERVICE
//     // --------------------------------------------------
//     BLEService *service = server->createService(SERVICE_UUID);

//     // ==================================================
//     // RX CHARACTERISTIC
//     // ==================================================
//     pRxCharacteristic = service->createCharacteristic(
//         RX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_WRITE_NR
//     );

//     pRxCharacteristic->setCallbacks(
//         new CarCharacteristicCallbacks()
//     );

//     // ==================================================
//     // TX CHARACTERISTIC
//     // ==================================================
//     pTxCharacteristic = service->createCharacteristic(
//         TX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_NOTIFY
//     );

//     pTxCharacteristic->addDescriptor(new BLE2902());

//     // --------------------------------------------------
//     // START SERVICE
//     // --------------------------------------------------
//     service->start();

//     // --------------------------------------------------
//     // ADVERTISING
//     // --------------------------------------------------
//     BLEAdvertising *advertising = BLEDevice::getAdvertising();

//     advertising->addServiceUUID(SERVICE_UUID);

//     advertising->setScanResponse(true);

//     // Better Android compatibility
//     advertising->setMinPreferred(0x06);
//     advertising->setMaxPreferred(0x12);

//     advertising->start();

//     // --------------------------------------------------
//     Serial.println("[BLE] BLE Started");
//     Serial.println("[BLE] Device Name: ESP32_CAR");
// }

// // ======================================================
// // SEND TELEMETRY
// // ======================================================
// void BLECar::sendTelemetry(SYSTEM_CASES state) {

//     if (!deviceConnected) {
//         return;
//     }

//     static unsigned long lastSend = 0;

//     unsigned long now = millis();

//     if (now - lastSend < TELEMETRY_INTERVAL_MS) {
//         return;
//     }

//     lastSend = now;

//     // ==================================================
//     // STATE STRING
//     // ==================================================
//     const char* stateStr;

//     switch (state) {

//         case SYSTEM_CHECK:
//             stateStr = "CHECK";
//             break;

//         case SYSTEM_IDLE:
//             stateStr = "IDLE";
//             break;

//         case SYSTEM_READY:
//             stateStr = "READY";
//             break;

//         case SYSTEM_NORMAL:
//             stateStr = "NORMAL";
//             break;

//         case SYSTEM_CRASH:
//             stateStr = "CRASH";
//             break;

//         case SYSTEM_EMERGENCY:
//             stateStr = "EMERGENCY";
//             break;

//         case SYSTEM_OBSTACLESTOP:
//             stateStr = "OBSTACLE";
//             break;

//         case SYSTEM_SAFESTOP:
//             stateStr = "SAFESTOP";
//             break;

//         case SYSTEM_EXCEEDED_TRIALS:
//             stateStr = "TRIALS_EXCEEDED";
//             break;

//         case SYSTEM_CHECK_FAIL:
//             stateStr = "CHECK_FAIL";
//             break;

//         default:
//             stateStr = "UNKNOWN";
//             break;
//     }

//     // ==================================================
//     // TELEMETRY BUFFER
//     // ==================================================
//     char buffer[64];

//     snprintf(
//         buffer,
//         sizeof(buffer),
//         "STATE:%s\n",
//         stateStr
//     );

//     // ==================================================
//     // SEND NOTIFICATION
//     // ==================================================
//     pTxCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));

//     pTxCharacteristic->notify();

//     Serial.print("[BLE] TX -> ");
//     Serial.println(buffer);
// }

// // ======================================================
// // HANDLE COMMANDS
// // ======================================================
// void BLECar::handleCommand(char cmd) {

//     switch (cmd) {

//         // ==================================================
//         // START / READY
//         // ==================================================
//         case 'P':

//             if (systemState != SYSTEM_IDLE) {

//                 Serial.println("[BLE] P ignored (not IDLE)");
//                 return;
//             }

//             if (!selfTestDone) {

//                 Serial.println("[BLE] SELF TEST NOT DONE");
//                 return;
//             }

//             if (!selfTestPassed) {

//                 Serial.println("[BLE] SELF TEST FAILED");
//                 return;
//             }

//             systemState = SYSTEM_READY;

//             Serial.println("[BLE] SYSTEM_READY");

//             break;

//         // ==================================================
//         // FAILED TRIAL
//         // ==================================================
//         case 'F':

//             Trials++;

//             Serial.print("[BLE] Trials = ");
//             Serial.println(Trials);

//             if (Trials >= MaxTrials) {

//                 systemState = SYSTEM_EXCEEDED_TRIALS;

//                 Serial.println("[BLE] SYSTEM_EXCEEDED_TRIALS");
//             }

//             break;

//         // ==================================================
//         // EMERGENCY
//         // ==================================================
//         case 'E':

//             systemState = SYSTEM_EMERGENCY;

//             Serial.println("[BLE] SYSTEM_EMERGENCY");

//             break;

//         // ==================================================
//         // MOTOR START
//         // ==================================================
//         case 'N':

//             if (systemState != SYSTEM_READY) {

//                 Serial.println("[BLE] N ignored (not READY)");
//                 return;
//             }

//             Set_Motor_Speed(250);

//             systemState = SYSTEM_NORMAL;

//             Serial.println("[BLE] Motor Started");

//             break;

//         // ==================================================
//         // STOP MOTOR
//         // ==================================================
//         case 'S':

//             Set_Motor_Speed(0);

//             systemState = SYSTEM_IDLE;

//             Serial.println("[BLE] SYSTEM_IDLE");

//             break;

//         // ==================================================
//         // UNKNOWN COMMAND
//         // ==================================================
//         default:

//             Serial.println("[BLE] Unknown command");

//             break;
//     }
// }

// // ======================================================
// // AUTO RECONNECT HANDLER
// // ======================================================
// void BLECar::handleReconnect() {

//     static bool advertising = false;

//     if (!deviceConnected && !advertising) {

//         delay(200);

//         BLEDevice::startAdvertising();

//         advertising = true;

//         Serial.println("[BLE] Waiting for reconnection...");
//     }

//     if (deviceConnected) {

//         advertising = false;
//     }
// }

// // ======================================================
// // CONNECTION STATUS
// // ======================================================
// bool BLECar::isConnected() {

//     return deviceConnected;
// }

// // ======================================================
// // GLOBAL OBJECT
// // ======================================================
// BLECar bleCar;




// // // the new code with 2 services on same phone
// #include "BLE.h"

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

// #include "Motor.h"

// // ======================================================
// // POINTERS (DRIVER)
// // ======================================================
// static BLECharacteristic *pRxCharacteristic = nullptr;
// static BLECharacteristic *pTxCharacteristic = nullptr;

// // ======================================================
// // POINTERS (SIMULATION)
// // ======================================================
// #if SIMULATION_BLE_ENABLED
// static BLECharacteristic *pSimRx = nullptr;
// static BLECharacteristic *pSimTx = nullptr;
// #endif

// static BLECar *instancePtr = nullptr;

// #define TELEMETRY_INTERVAL_MS 500

// // ======================================================
// // SERVER CALLBACKS
// // ======================================================
// class CarServerCallbacks : public BLEServerCallbacks {

//     void onConnect(BLEServer* pServer) override {
//         if (instancePtr)
//             instancePtr->deviceConnected = true;

//         Serial.println("[BLE] CONNECTED");
//     }

//     void onDisconnect(BLEServer* pServer) override {
//         if (instancePtr)
//             instancePtr->deviceConnected = false;

//         Serial.println("[BLE] DISCONNECTED");

//         delay(200);
//         pServer->startAdvertising();
//     }
// };

// // ======================================================
// // DRIVER CALLBACKS
// // ======================================================
// class CarCharacteristicCallbacks : public BLECharacteristicCallbacks {

//     void onWrite(BLECharacteristic *c) override {

//         String v = c->getValue();
//         if (v.length() == 0) return;

//         BLECar::handleCommand(v[0]);
//     }
// };

// #if SIMULATION_BLE_ENABLED

// // ======================================================
// // SIM CALLBACKS
// // ======================================================
// class SimCallbacks : public BLECharacteristicCallbacks {

//     void onWrite(BLECharacteristic *c) override {

//         String v = c->getValue();
//         if (v.length() == 0) return;

//         BLECar::handleSimCommand(v[0]);
//     }
// };

// #endif

// // ======================================================
// // CONSTRUCTOR
// // ======================================================
// BLECar::BLECar() {
//     deviceConnected = false;
//     instancePtr = this;
// }

// // ======================================================
// // BEGIN
// // ======================================================
// void BLECar::begin() {

//     BLEDevice::init("ESP32_CAR");
//     BLEDevice::setPower(ESP_PWR_LVL_P9);

//     BLEServer *server = BLEDevice::createServer();
//     server->setCallbacks(new CarServerCallbacks());

//     // ==================================================
//     // DRIVER SERVICE (UNCHANGED)
//     // ==================================================
//     BLEService *service = server->createService(SERVICE_UUID);

//     pRxCharacteristic = service->createCharacteristic(
//         RX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_WRITE_NR
//     );

//     pRxCharacteristic->setCallbacks(new CarCharacteristicCallbacks());

//     pTxCharacteristic = service->createCharacteristic(
//         TX_CHAR_UUID,
//         BLECharacteristic::PROPERTY_NOTIFY
//     );

//     pTxCharacteristic->addDescriptor(new BLE2902());

//     service->start();

//     // ==================================================
//     // SIMULATION SERVICE (ADDED ONLY)
//     // ==================================================
// #if SIMULATION_BLE_ENABLED

//     BLEService *simService = server->createService(SIM_SERVICE_UUID);

//     pSimRx = simService->createCharacteristic(
//         SIM_RX_UUID,
//         BLECharacteristic::PROPERTY_WRITE |
//         BLECharacteristic::PROPERTY_WRITE_NR
//     );

//     pSimRx->setCallbacks(new SimCallbacks());

//     pSimTx = simService->createCharacteristic(
//         SIM_TX_UUID,
//         BLECharacteristic::PROPERTY_NOTIFY
//     );

//     pSimTx->addDescriptor(new BLE2902());

//     simService->start();

// #endif

//     // ==================================================
//     // ADVERTISING
//     // ==================================================
//     BLEAdvertising *adv = BLEDevice::getAdvertising();

//     adv->addServiceUUID(SERVICE_UUID);

// #if SIMULATION_BLE_ENABLED
//     adv->addServiceUUID(SIM_SERVICE_UUID);
// #endif

//     adv->setScanResponse(true);
//     adv->setMinPreferred(0x06);
//     adv->setMaxPreferred(0x12);

//     adv->start();

//     Serial.println("[BLE] READY");
// }

// // ======================================================
// // TELEMETRY (UNCHANGED LOGIC)
// // ======================================================
// void BLECar::sendTelemetry(SYSTEM_CASES state) {

//     if (!deviceConnected) return;

//     static unsigned long last = 0;
//     if (millis() - last < TELEMETRY_INTERVAL_MS) return;
//     last = millis();

//     const char* stateStr;

//     switch (state) {

//         case SYSTEM_CHECK:
//             stateStr = "CHECK";
//             break;

//         case SYSTEM_IDLE:
//             stateStr = "IDLE";
//             break;

//         case SYSTEM_READY:
//             stateStr = "READY";
//             break;

//         case SYSTEM_NORMAL:
//             stateStr = "NORMAL";
//             break;

//         case SYSTEM_CRASH:
//             stateStr = "CRASH";
//             break;

//         case SYSTEM_EMERGENCY:
//             stateStr = "EMERGENCY";
//             break;

//         case SYSTEM_OBSTACLESTOP:
//             stateStr = "OBSTACLE";
//             break;

//         case SYSTEM_SAFESTOP:
//             stateStr = "SAFESTOP";
//             break;

//         case SYSTEM_EXCEEDED_TRIALS:
//             stateStr = "TRIALS_EXCEEDED";
//             break;

//         case SYSTEM_CHECK_FAIL:
//             stateStr = "CHECK_FAIL";
//             break;

//         default:
//             stateStr = "UNKNOWN";
//             break;
//     }

//     char buf[64];
//     snprintf(buf, sizeof(buf), "STATE:%s\n", stateStr);

//     pTxCharacteristic->setValue((uint8_t*)buf, strlen(buf));
//     pTxCharacteristic->notify();

// #if SIMULATION_BLE_ENABLED
//     pSimTx->setValue((uint8_t*)buf, strlen(buf));
//     pSimTx->notify();
// #endif
// }

// // ======================================================
// // DRIVER COMMANDS
// // ======================================================
// void BLECar::handleCommand(char cmd) {

//      switch (cmd) {

//         // ==================================================
//         // START / READY
//         // ==================================================
//         case 'P':

//             if (systemState != SYSTEM_IDLE) {

//                 Serial.println("[BLE] P ignored (not IDLE)");
//                 return;
//             }

//             if (!selfTestDone) {

//                 Serial.println("[BLE] SELF TEST NOT DONE");
//                 return;
//             }

//             if (!selfTestPassed) {

//                 Serial.println("[BLE] SELF TEST FAILED");
//                 return;
//             }

//             systemState = SYSTEM_READY;

//             Serial.println("[BLE] SYSTEM_READY");

//             break;

//         // ==================================================
//         // FAILED TRIAL
//         // ==================================================
//         case 'F':

//             Trials++;

//             Serial.print("[BLE] Trials = ");
//             Serial.println(Trials);

//             if (Trials >= MaxTrials) {

//                 systemState = SYSTEM_EXCEEDED_TRIALS;

//                 Serial.println("[BLE] SYSTEM_EXCEEDED_TRIALS");
//             }

//             break;

//         // ==================================================
//         // EMERGENCY
//         // ==================================================
//         case 'E':

//             systemState = SYSTEM_EMERGENCY;

//             Serial.println("[BLE] SYSTEM_EMERGENCY");

//             break;

//         // ==================================================
//         // UNKNOWN COMMAND
//         // ==================================================
//         default:

//             Serial.println("[BLE] Unknown command");

//             break;
//     }
// }

// // ======================================================
// // SIMULATION COMMANDS
// // ======================================================
// #if SIMULATION_BLE_ENABLED

// void BLECar::handleSimCommand(char cmd) {

//     switch(cmd) {

//         case 'M':   // MOVE
//             motorLocked = false;
//             Set_Motor_Speed(250);
//             Serial.println("[SIM] MOVE");
//             break;

//         case 'S':   // STOP
//             motorLocked = true;
//             Set_Motor_Speed(0);
//             Serial.println("[SIM] STOP");
//             break;
//     }
// }

// #endif

// // ======================================================
// void BLECar::handleReconnect() {

//     static bool adv = false;

//     if (!deviceConnected && !adv) {
//         delay(200);
//         BLEDevice::startAdvertising();
//         adv = true;
//     }

//     if (deviceConnected)
//         adv = false;
// }

// // ======================================================
// bool BLECar::isConnected() {
//     return deviceConnected;
// }

// // ======================================================



// 2 services but must be on 2 devices one is not gonna work as expected 
// BLECar bleCar;
#include "BLE.h"
#include <NimBLEDevice.h>
#include "Motor.h"

// ======================================================
// CHARACTERISTICS
// ======================================================
static NimBLECharacteristic *pTxCharacteristic = nullptr;
static NimBLECharacteristic *pSimTx = nullptr;

static NimBLECharacteristic *pRxCharacteristic = nullptr;
static NimBLECharacteristic *pSimRx = nullptr;

// ======================================================
// CONNECTION TRACKING
// ======================================================
static uint16_t driverConn = 0;
static uint16_t simConn = 0;

#define TELEMETRY_INTERVAL_MS 500

// ======================================================
// SERVER CALLBACKS
// ======================================================
class CarServerCallbacks : public NimBLEServerCallbacks {

    void onConnect(NimBLEServer* pServer,
                   NimBLEConnInfo& connInfo) override {

        uint16_t id = connInfo.getConnHandle();

        Serial.println("[BLE] DEVICE CONNECTED");

        // First connected phone = Driver
        if (driverConn == 0) {

            driverConn = id;
            Serial.println("[BLE] DRIVER ASSIGNED");
        }

        // Second connected phone = Simulation
        else if (simConn == 0) {

            simConn = id;
            Serial.println("[BLE] SIM ASSIGNED");
        }

        else {

            Serial.println("[BLE] EXTRA DEVICE IGNORED");
        }

        // IMPORTANT:
        // Keep advertising so second phone can still connect
        NimBLEDevice::startAdvertising();
    }

    void onDisconnect(NimBLEServer* pServer,
                      NimBLEConnInfo& connInfo,
                      int reason) override {

        uint16_t id = connInfo.getConnHandle();

        if (id == driverConn)
            driverConn = 0;

        if (id == simConn)
            simConn = 0;

        Serial.println("[BLE] DEVICE DISCONNECTED");

        // Restart advertising
        NimBLEDevice::startAdvertising();
    }
};

// ======================================================
// DRIVER CALLBACK
// ======================================================
class CarCharacteristicCallbacks
    : public NimBLECharacteristicCallbacks {

    void onWrite(NimBLECharacteristic *c,
                 NimBLEConnInfo &connInfo) override {

        std::string v = c->getValue();

        if (v.empty())
            return;

        BLECar::handleCommand(v[0]);
    }
};

// ======================================================
// SIM CALLBACK
// ======================================================
class SimCallbacks
    : public NimBLECharacteristicCallbacks {

    void onWrite(NimBLECharacteristic *c,
                 NimBLEConnInfo &connInfo) override {

        std::string v = c->getValue();

        if (v.empty())
            return;

        BLECar::handleSimCommand(v[0]);
    }
};

// ======================================================
// CONSTRUCTOR
// ======================================================
BLECar::BLECar() {

    deviceConnected = false;
}

// ======================================================
// BEGIN
// ======================================================
void BLECar::begin() {

    // =========================
    // INIT
    // =========================
    NimBLEDevice::init("ESP32_CAR");

    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    NimBLEDevice::setMTU(256);

    // =========================
    // SERVER
    // =========================
    NimBLEServer *server =
        NimBLEDevice::createServer();

    server->setCallbacks(new CarServerCallbacks());

    // ==================================================
    // DRIVER SERVICE
    // ==================================================
    NimBLEService *service =
        server->createService(SERVICE_UUID);

    pRxCharacteristic =
        service->createCharacteristic(
            RX_CHAR_UUID,
            NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::WRITE_NR
        );

    pRxCharacteristic->setCallbacks(
        new CarCharacteristicCallbacks()
    );

    pTxCharacteristic =
        service->createCharacteristic(
            TX_CHAR_UUID,
            NIMBLE_PROPERTY::NOTIFY
        );

    service->start();

#if SIMULATION_BLE_ENABLED

    // ==================================================
    // SIM SERVICE
    // ==================================================
    NimBLEService *simService =
        server->createService(SIM_SERVICE_UUID);

    pSimRx =
        simService->createCharacteristic(
            SIM_RX_UUID,
            NIMBLE_PROPERTY::WRITE |
            NIMBLE_PROPERTY::WRITE_NR
        );

    pSimRx->setCallbacks(new SimCallbacks());

    pSimTx =
        simService->createCharacteristic(
            SIM_TX_UUID,
            NIMBLE_PROPERTY::NOTIFY
        );

    simService->start();

#endif

    // ==================================================
    // ADVERTISING
    // ==================================================
    NimBLEAdvertising *adv =
        NimBLEDevice::getAdvertising();

    adv->addServiceUUID(SERVICE_UUID);

#if SIMULATION_BLE_ENABLED
    adv->addServiceUUID(SIM_SERVICE_UUID);
#endif

    adv->start();

    Serial.println("[BLE] READY");
}

// ======================================================
// TELEMETRY
// ======================================================
void BLECar::sendTelemetry(SYSTEM_CASES state) {

    static uint32_t last = 0;

    if (millis() - last < TELEMETRY_INTERVAL_MS)
        return;

    last = millis();

    const char* stateStr;

    switch (state) {

        case SYSTEM_CHECK:
            stateStr = "CHECK";
            break;

        case SYSTEM_IDLE:
            stateStr = "IDLE";
            break;

        case SYSTEM_READY:
            stateStr = "READY";
            break;

        case SYSTEM_NORMAL:
            stateStr = "NORMAL";
            break;

        case SYSTEM_CRASH:
            stateStr = "CRASH";
            break;

        case SYSTEM_EMERGENCY:
            stateStr = "EMERGENCY";
            break;

        case SYSTEM_OBSTACLESTOP:
            stateStr = "OBSTACLE";
            break;

        case SYSTEM_SAFESTOP:
            stateStr = "SAFESTOP";
            break;

        case SYSTEM_EXCEEDED_TRIALS:
            stateStr = "TRIALS";
            break;

        case SYSTEM_CHECK_FAIL:
            stateStr = "FAIL";
            break;

        case SYSTEM_END_TRIP:
            stateStr = "END_TRIP";
            break;

        default:
            stateStr = "UNKNOWN";
            break;
    }

    char buf[64];

    snprintf(buf,
             sizeof(buf),
             "STATE:%s\n",
             stateStr);

    // Driver phone
    if (driverConn != 0) {

        pTxCharacteristic->setValue(
            (uint8_t*)buf,
            strlen(buf)
        );

        pTxCharacteristic->notify();
    }

#if SIMULATION_BLE_ENABLED

    // Simulation phone
    if (simConn != 0) {

        pSimTx->setValue(
            (uint8_t*)buf,
            strlen(buf)
        );

        pSimTx->notify();
    }

#endif
}

// ======================================================
// DRIVER COMMANDS
// ======================================================
void BLECar::handleCommand(char cmd) {

    if (systemState == SYSTEM_END_TRIP) {

        Serial.println(
            "[BLE] TRIP ENDED - IGNORING"
        );

        return;
    }

    switch (cmd) {

        // =============================
        // READY
        // =============================
        case 'P':

            if (systemState == SYSTEM_IDLE) {

                systemState = SYSTEM_READY;

                Serial.println("[BLE] READY");
            }

            break;

        // =============================
        // FAILED TRIAL
        // =============================
        case 'F':

            Trials++;

            Serial.print("[BLE] Trials = ");
            Serial.println(Trials);

            break;

        // =============================
        // EMERGENCY
        // =============================
        case 'E':

            systemState = SYSTEM_EMERGENCY;

            Serial.println("[BLE] EMERGENCY");

            break;

        // =============================
        // END TRIP
        // =============================
        case 'T':

            systemState = SYSTEM_END_TRIP;

            // motorLocked = true;

            // Set_Motor_Speed(0);

            Serial.println("[BLE] END TRIP");

            break;

        default:

            Serial.println("[BLE] UNKNOWN COMMAND");

            break;
    }
}

// ======================================================
// SIM COMMANDS
// ======================================================
#if SIMULATION_BLE_ENABLED

void BLECar::handleSimCommand(char cmd) {

    if (systemState == SYSTEM_END_TRIP)
        return;

    switch (cmd) {

        // =============================
        // MOVE
        // =============================
        case 'M':

            motorLocked = false;

            Set_Motor_Speed(250);

            Serial.println("[SIM] MOVE");

            break;

        // =============================
        // STOP
        // =============================
        case 'S':

            motorLocked = true;

            Set_Motor_Speed(0);

            Serial.println("[SIM] STOP");

            break;

        default:

            Serial.println("[SIM] UNKNOWN");

            break;
    }
}

#endif

// ======================================================
void BLECar::handleReconnect() {

    NimBLEDevice::startAdvertising();
}

// ======================================================
bool BLECar::isConnected() {

    return (driverConn != 0 ||
            simConn != 0);
}

// ======================================================
BLECar bleCar;
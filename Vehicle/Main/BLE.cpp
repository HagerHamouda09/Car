// BLE.cpp

#include "BLE.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Motor.h"

extern int Trials;
// --------------------------------------------------
// Global BLE pointers
// --------------------------------------------------
static BLECharacteristic *pRxCharacteristic = nullptr;
static BLECharacteristic *pTxCharacteristic = nullptr;
static BLECar *instancePtr = nullptr;

// --------------------------------------------------
#define TELEMETRY_INTERVAL_MS 500

// --------------------------------------------------
// CONNECTION CALLBACKS
// --------------------------------------------------
class CarServerCallbacks : public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) override {
        if (instancePtr) {
            instancePtr->deviceConnected = true;
        }

        Serial.println("[BLE] Phone connected");
    }

    void onDisconnect(BLEServer* pServer) override {
        if (instancePtr) {
            instancePtr->deviceConnected = false;
        }

        Serial.println("[BLE] Phone disconnected");

        BLEDevice::startAdvertising();
    }

//     void BLECar::handleReconnect() {
//     if (!deviceConnected) {
//         delay(500);
//         BLEDevice::startAdvertising();
//         Serial.println("[BLE] Restarting advertising...");
//     }
// }
};

// --------------------------------------------------
// RX CALLBACK
// --------------------------------------------------
class CarCharacteristicCallbacks : public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *characteristic) override {

        // ESP32 BLE library returns Arduino String
        String value = characteristic->getValue();

        if (value.length() > 0) {

            char cmd = value[0];

            Serial.print("[BLE] RX Command: ");
            Serial.println(cmd);

            BLECar::handleCommand(cmd);
        }
    }
};

// --------------------------------------------------
// CONSTRUCTOR
// --------------------------------------------------
BLECar::BLECar() {
    deviceConnected = false;
    instancePtr = this;
}

// --------------------------------------------------
// BLE INIT
// --------------------------------------------------
void BLECar::begin() {

    BLEDevice::init("ESP32_CAR");

    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(new CarServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    // ---------------- RX CHARACTERISTIC ----------------
    // Phone writes commands here
    pRxCharacteristic = service->createCharacteristic(
        RX_CHAR_UUID,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_WRITE_NR
    );

    pRxCharacteristic->setCallbacks(
        new CarCharacteristicCallbacks()
    );

    // ---------------- TX CHARACTERISTIC ----------------
    // ESP32 sends telemetry here
    pTxCharacteristic = service->createCharacteristic(
        TX_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pTxCharacteristic->addDescriptor(new BLE2902());

    // --------------------------------------------------
    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();

    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("[BLE] BLE Started");
    Serial.println("[BLE] Device Name: ESP32_CAR");
}

// --------------------------------------------------
// SEND TELEMETRY
// --------------------------------------------------
void BLECar::sendTelemetry(
    SYSTEM_CASES state
    // float speed,
    // float distance
) {

    if (!deviceConnected) {
        return;
    }

    static unsigned long lastSend = 0;

    unsigned long now = millis();

    if (now - lastSend < TELEMETRY_INTERVAL_MS) {
        return;
    }

    lastSend = now;

    // --------------------------------------------------
    // Convert enum state to string
    // --------------------------------------------------
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
            stateStr = "TRIALS_EXCEEDED";
            break;

        case SYSTEM_CHECK_FAIL:
            stateStr = "CHECK_FAIL";
            break;

        default:
            stateStr = "UNKNOWN";
            break;
    }

    // --------------------------------------------------
    // Build telemetry packet
    // --------------------------------------------------
    char buffer[64];

    snprintf(
        buffer,
        sizeof(buffer),
        // "STATE:%s,SPD:%.1f,DIST:%.1f\n",
        "STATE:%s\n",

        stateStr
        // speed,
        // distance
    );

    // --------------------------------------------------
    // Notify phone
    // --------------------------------------------------
    pTxCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
    pTxCharacteristic->notify();

    Serial.print("[BLE] TX -> ");
    Serial.println(buffer);
}

// --------------------------------------------------
// HANDLE COMMANDS
// --------------------------------------------------
void BLECar::handleCommand(char cmd) {

    switch (cmd) {

        // ----------------------------------------------
        // EMERGENCY
        // ----------------------------------------------
        case 'P':

            if (systemState != SYSTEM_IDLE)
                {
                    Serial.println("[BLE] P ignored (not in IDLE)");
                    return;
                }

            if (!selfTestDone)
            {
                Serial.println("[BLE] SELF TEST NOT DONE YET");
                return;
            }

            if (!selfTestPassed)
            {
                Serial.println("[BLE] SELF TEST FAILED → IGNORING P");
                return;
            }

            systemState = SYSTEM_READY;
            Serial.println("[BLE] SYSTEM_READY");

            break;
            
        case 'F':

            Trials++;
            if(Trials>=MaxTrials)
                {
                    systemState = SYSTEM_EXCEEDED_TRIALS;
                }
            break;
        
        case 'E':

            systemState = SYSTEM_EMERGENCY;

            Serial.println("[BLE] SYSTEM_EMERGENCY");

            break;
        

        case 'N':

        if (systemState != SYSTEM_READY)
        {
            Serial.println("[BLE] N ignored (not in READY)");
            return;
        }

        Set_Motor_Speed(250);

        Serial.println("[BLE] Motor start command");

        break;


        // // ----------------------------------------------
        // // RESET AFTER CRASH
        // // ----------------------------------------------
        // case 'R':

        //     // motorLocked = false;
        //     systemState = SYSTEM_CRASH;

        //     Serial.println("[BLE] RESET -> SYSTEM_CRASH");

        //     break;

        // // ----------------------------------------------
        // // RESUME NORMAL
        // // ----------------------------------------------
        // case 'N':

        //     // motorLocked = false;
        //     systemState = SYSTEM_NORMAL;

        //     Serial.println("[BLE] SYSTEM_NORMAL");

        //     break;

        // ----------------------------------------------
        // // IDLE / STOP
        // // ----------------------------------------------
        // case 'I':

        //     // motorLocked = true;
        //     systemState = SYSTEM_IDLE;

        //     Serial.println("[BLE] SYSTEM_IDLE");

        //     break;

        // // ----------------------------------------------
        default:

            Serial.println("[BLE] Unknown command");

            break;
    }
}

// --------------------------------------------------
// CONNECTION STATUS
// --------------------------------------------------
bool BLECar::isConnected() {
    return deviceConnected;
}

// --------------------------------------------------
// GLOBAL OBJECT
// --------------------------------------------------
BLECar bleCar;







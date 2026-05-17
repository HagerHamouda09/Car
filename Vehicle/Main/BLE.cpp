#include "BLECar.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ---------------- UUIDs ----------------
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

// ---------------- GLOBAL POINTER ----------------
static BLECharacteristic *pCharacteristic;
static BLECar *instancePtr = nullptr;

// ---------------- CALLBACK: CONNECTION ----------------
class CarServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        if (instancePtr) instancePtr->deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        if (instancePtr) instancePtr->deviceConnected = false;
        BLEDevice::startAdvertising();
    }
};

// ---------------- CALLBACK: DATA RECEIVED ----------------
class CarCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
        std::string value = characteristic->getValue();

        if (value.length() > 0) {
            char cmd = value[0];
            BLECar::handleCommand(cmd);
        }
    }
};

// ---------------- CONSTRUCTOR ----------------
BLECar::BLECar() {
    deviceConnected = false;
    instancePtr = this;
}

// ---------------- INIT BLE ----------------
void BLECar::begin() {
    BLEDevice::init("ESP32_CAR");

    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(new CarServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    pCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(new CarCharacteristicCallbacks());

    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->start();

    Serial.println("BLE Car Module Started");
}

// ---------------- COMMAND HANDLER ----------------
void BLECar::handleCommand(char cmd) {
    switch (cmd) {

        case 'F':
            Serial.println("Forward");
            // TODO: call your motor function
            // moveForward();
            break;

        case 'B':
            Serial.println("Backward");
            // moveBackward();
            break;

        case 'L':
            Serial.println("Left");
            // turnLeft();
            break;

        case 'R':
            Serial.println("Right");
            // turnRight();
            break;

        case 'S':
            Serial.println("Stop");
            // stopCar();
            break;

        default:
            Serial.println("Unknown CMD");
            break;
    }
}

// ---------------- OPTIONAL UPDATE ----------------
void BLECar::update() {
    // future use (telemetry, heartbeat, etc.)
}

// ---------------- STATUS ----------------
bool BLECar::isConnected() {
    return deviceConnected;
}


// // How to use
// #include "BLECar.h"

// BLECar car;

// void setup() {
//     Serial.begin(115200);

//     car.begin();   // 🚗 start BLE module
// }

// void loop() {
//     car.update();  // optional
// }
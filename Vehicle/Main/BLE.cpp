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

        ble_handleCommand(v[0]);
    }
};

// ======================================================
// SIM CALLBACK
// ======================================================
#if SIMULATION_BLE_ENABLED

class SimCallbacks
    : public NimBLECharacteristicCallbacks {

    void onWrite(NimBLECharacteristic *c,
                 NimBLEConnInfo &connInfo) override {

        std::string v = c->getValue();

        if (v.empty())
            return;

        ble_handleSimCommand(v[0]);
    }
};

#endif




// ======================================================
// BEGIN
// ======================================================
void ble_begin() {

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
void ble_sendData(SYSTEM_CASES state)
{
    static uint32_t last = 0;

    if (millis() - last < TELEMETRY_INTERVAL_MS)
        return;

    last = millis();

    // Driver phone:
    // Don't send anything here anymore (or send your single-letter messages
    // using sendSystemCheck(), sendCrash(), etc.)

#if SIMULATION_BLE_ENABLED

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

    if (simConn != 0) {

        pSimTx->setValue((uint8_t*)buf, strlen(buf));
        pSimTx->notify();
    }

#endif
}


void ble_send_systemcheck(bool passed)
{
    if (!driverConn || pTxCharacteristic == nullptr) return;

    static unsigned long lastSend = 0;
    if (millis() - lastSend < 1000) return;
    lastSend = millis();

    const char* msg = passed ? "P\n" : "F\n";

    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();
}


void ble_sendCrash()
{
    if (!driverConn || pTxCharacteristic == nullptr) return;

    static unsigned long lastSend = 0;
    if (millis() - lastSend < 1000) return;
    lastSend = millis();

    const char* msg = "C\n";

    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();
}


// ======================================================
// DRIVER COMMANDS
// ======================================================
void ble_handleCommand(char cmd) {

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
            
            Serial.println("[BLE] END TRIP");

            break;

        //  =============================
        // Driver misuse
        // =============================

        case 'A':

            systemState = SYSTEM_MISUSE;
            Serial.println("[BLE] Driver Misuse");
           
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

void ble_handleSimCommand(char cmd) {

    if (systemState == SYSTEM_END_TRIP)
        return;

    switch (cmd) {

        // =============================
        // MOVE
        // =============================
            case 'M':

        // Serial.print("Before=");
        // Serial.println(motorLocked);

        motorLocked = false;

        // Serial.print("After=");
        // Serial.println(motorLocked);

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

// // ======================================================
// void BLECar::handleReconnect() {

//     NimBLEDevice::startAdvertising();
// }

// // ======================================================
bool ble_isConnected() {

    return (driverConn != 0 ||
            simConn != 0);
}

// ======================================================

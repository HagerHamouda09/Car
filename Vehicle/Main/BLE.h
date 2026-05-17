#ifndef BLE_CAR_H
#define BLE_CAR_H

#include <Arduino.h>

// ---------------- MAIN CLASS ----------------
class BLECar {
public:
    BLECar();

    void begin();     // initialize BLE
    void update();    // optional future use (loop hook)

    bool isConnected();

private:
    bool deviceConnected;

    // Internal handlers (BLE callbacks)
    static void handleCommand(char cmd);

    friend class CarServerCallbacks;
    friend class CarCharacteristicCallbacks;
};

#endif
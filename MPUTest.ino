
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("I2C SCAN START");

  Wire.begin(21, 22);

  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("FOUND DEVICE AT 0x");
      Serial.println(addr, HEX);
    }
  }

  Serial.println("DONE");
}

void loop() {}



# GuardianDrive – Vehicle Add-On Safety & Control Module

The GuardianDrive Vehicle Add-On Module is an ADAS embedded control unit designed to monitor vehicle kinematics, execute obstacle/collision mitigation, and enact multi-stage emergency interventions (actuator braking and battery power disconnect) governed by driver health states.

---

## Hardware Architecture & Components

* **Microcontroller:** ESP32 Dev Module (ESP-WROOM-32 dual-core)
* **Inertial Measurement Unit (IMU):** MPU-6500 (6-axis gyro/accelerometer for real-time crash and impact detection)
* **Obstacle Detection:** Ultrasonic Sensor (HC-SR04 for forward distance tracking)
* **Speed / Wheel Tick Sensing:** Hall Effect Sensor + Neodymium disc magnets
* **Powertrain & Actuation:**
  * DC Gear Motors driven by HW-166 (L298N/equivalent) Motor Driver
  * Servo Motor configured as a mechanical brake pedal actuator
  * 5V Relay Module configured as an electronic "Battery Disconnect Switch" (Battery Killer)
* **Alert Peripherals:** High-decibel Piezo Buzzer and high-intensity hazard LEDs
* **Power Regulation:** LM2596HVS step-down buck converter powering logic and motors from dual 18650 Li-ion cells

<img width="316" height="171" alt="image" src="https://github.com/user-attachments/assets/527f95e7-cd96-4faa-b82b-fda75c59a061" />

---

## Embedded Control & ADAS Logic

* **Pre-Drive Ignition Interlock:** The car remains immobilized by default via a battery-disconnect relay until the mobile app transmits a `Car Unlock` signal following a validated driver biometric pre-check.
* **Active Crash Detection:** Continuously samples the MPU-6500 for sudden G-force spikes. On impact, the firmware instantly triggers hazard LEDs/buzzers and reports a `CRASH` event over BLE.
* **Obstacle Distance Mitigation:**
  * Safe distance breach: Initiates progressive DC motor deceleration.
  * Critical distance breach: Enacts an immediate full stop.
* **Multi-Stage Autonomous Emergency Stopping (AES):**
  1. Upon receiving an `EMERGENCY_STOP` payload (unresponsive driver or SOS event), the servo motor rotates to actuate the braking mechanism.
  2. Simultaneously triggers vehicle hazard flashers and buzzer patterns.
  3. Once speed drops below a safe threshold, toggles the Relay to completely cut off vehicle battery power.

---

## Setup & Deployment

1. **Toolchain:** Open the firmware project in PlatformIO or Arduino IDE.
2. **Configuration:** Verify pin mappings in `config.h`:
   * `RELAY_PIN`: Power cutoff relay
   * `SERVO_PIN`: Brake pedal actuator servo
   * `TRIG_PIN` / `ECHO_PIN`: Ultrasonic sensor
   * `MPU_SDA` / `MPU_SCL`: I2C channels
   * `MOTOR_PWM_A` / `MOTOR_PWM_B`: DC motor speed regulation
3. **Flashing:** Connect the ESP32 Dev Module via Micro-USB, select **ESP32 Dev Module**, and upload the firmware.
4. **Integration Run:** Power the car module; the onboard BLE service will advertise its UUID and await the GuardianDrive mobile app connection.


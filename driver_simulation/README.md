# GuardianDrive – Driver & Vehicle Hardware Simulator

An internal mobile testing and emulation tool developed to simulate vehicle states, control commands, and telemetry payloads over Bluetooth Low Energy (BLE). This utility validates the vehicle hardware module and backend integration without requiring active on-road testing.

---

## Overview

The Driver Simulation App acts as a diagnostic testbench. It simulates driver interactions and hardware telemetry, allowing engineers to verify how the vehicle controller and mobile safety pipelines respond to critical transit states (e.g., motion tracking, manual stop requests, and high-G crash triggers).


<img width="720" height="1440" alt="image" src="https://github.com/user-attachments/assets/ea99a9f8-f18e-4168-bf93-dbf86de3f56a" />

<img width="720" height="1440" alt="image33" src="https://github.com/user-attachments/assets/94bcd492-9936-4f0e-8f19-8ac4dffc38e2" />

<img width="720" height="1440" alt="image102" src="https://github.com/user-attachments/assets/7e5cdc12-d113-4e21-a696-545e1ae31ade" />


<img width="720" height="1440" alt="image92" src="https://github.com/user-attachments/assets/86ee84df-8ce7-4c1f-bc57-046476f25344" />

---

## Key Features

* **BLE Handshake & Pairing:** Scans, connects, and maintains an active BLE link with the GuardianDrive vehicle module (ESP32 Dev Module).
* **Vehicle Motion Emulation:** Dispatches `MOVE` and `STOP` commands to test DC motor driving logic and speed control routines.
* **Crash State Emulation:** Injects a simulated impact (`CRASH`) event over Bluetooth to verify the vehicle's automated emergency responses (piezo buzzer activation, hazard lighting, and mobile incident alerts).
* **State Telemetry Verification:** Monitors and displays real-time state feedback transitions (`Disconnected` $\rightarrow$ `READY` $\rightarrow$ `CRASH`).

---

## Supported Simulation States & Commands

| State / Trigger | Target Action | Verification Metric |
| :--- | :--- | :--- |
| **CONNECT** | Initiates BLE pairing handshake | Connection status updates to `CONNECTED` |
| **MOVE** | Signals vehicle to begin motion | Verifies motor driver PWM execution |
| **STOP**| Signals manual deceleration | Validates brake servo/motor stop routines |
| **CRASH State** | Simulates extreme MPU acceleration impact | Triggers local buzzer, LEDs, and vehicle cutoff alert |

---

## Getting Started

### Prerequisites
* Flutter SDK (v3.x or later)
* Android device with Bluetooth 5.0+ and Location permissions enabled

### Installation & Run
1. Clone the repository and navigate to the project directory:
   ```bash
   git clone [https://github.com/YourOrg/GuardianDrive-DriverSimulator.git](https://github.com/YourOrg/GuardianDrive-DriverSimulator.git)
   cd GuardianDrive-DriverSimulator

```

2. Install dependencies:
```bash
flutter pub get

```


3. Deploy to a connected physical testing device:
```bash
flutter run

```

---

## Testing Workflow

1. Power on the vehicle prototype (ESP32 module).


2. Open the simulator app and tap **CONNECT** on the Bluetooth page.


3. Verify that the vehicle state switches to `READY` upon successful handshake.


4. Tap **MOVE** and **STOP** to confirm proper DC motor actuation.


5. Trigger **CRASH** to confirm instantaneous state transitions, alert generation, and safety cutoff sequences.


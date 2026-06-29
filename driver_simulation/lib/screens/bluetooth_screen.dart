import 'package:flutter/material.dart';
import '../services/simulation_ble_service.dart';

class BluetoothScreen extends StatefulWidget {
  const BluetoothScreen({super.key});

  @override
  State<BluetoothScreen> createState() => _BluetoothScreenState();
}

class _BluetoothScreenState extends State<BluetoothScreen> {
  final SimulationBleService ble = SimulationBleService.instance;

  bool connected = false;
  bool reconnecting = false;

  String vehicleState = "Not Connected";

  @override
  void initState() {
    super.initState();

    ble.connectionNotifier.addListener(updateConnection);
    ble.stateNotifier.addListener(updateVehicleState);

    // initial values
    connected = ble.isConnected;
    vehicleState = ble.stateNotifier.value;
  }

  void updateConnection() {
    if (!mounted) return;

    setState(() {
      connected = ble.connectionNotifier.value;
      reconnecting = ble.stateNotifier.value == "Reconnecting...";
    });
  }

  void updateVehicleState() {
    if (!mounted) return;

    setState(() {
      vehicleState = ble.stateNotifier.value;
    });
  }

  @override
  void dispose() {
    ble.connectionNotifier.removeListener(updateConnection);
    ble.stateNotifier.removeListener(updateVehicleState);
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        width: double.infinity,
        height: double.infinity,
        decoration: const BoxDecoration(
          gradient: RadialGradient(
            center: Alignment(0, -0.2),
            radius: 1.2,
            colors: [
              Color(0xFF123744),
              Color(0xFF0A1C27),
              Color(0xFF07131A),
              Color(0xFF050C11),
            ],
          ),
        ),
        child: SafeArea(
          child: Padding(
            padding: const EdgeInsets.all(24),
            child: SingleChildScrollView(
              child: Column(
                children: [
                  const SizedBox(height: 20),

                  const Text(
                    "DRIVER SIMULATION",
                    style: TextStyle(
                      color: Colors.white,
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 2,
                    ),
                  ),

                  const SizedBox(height: 10),

                  const Text(
                    "Bluetooth Control",
                    style: TextStyle(
                      color: Colors.white70,
                      fontSize: 17,
                    ),
                  ),

                  const SizedBox(height: 35),

                  // ===============================
                  // CONNECTION CARD
                  // ===============================
                  Container(
                    width: double.infinity,
                    padding: const EdgeInsets.all(18),
                    decoration: BoxDecoration(
                      color: Colors.white10,
                      borderRadius: BorderRadius.circular(18),
                      border: Border.all(
                        color: const Color(0xFF00D9FF),
                      ),
                    ),
                    child: Column(
                      children: [
                        const Text(
                          "Bluetooth Status",
                          style: TextStyle(
                            color: Colors.white70,
                            fontSize: 18,
                          ),
                        ),
                        const SizedBox(height: 12),
                        Text(
                          connected
                              ? "CONNECTED"
                              : reconnecting
                                  ? "RECONNECTING..."
                                  : "DISCONNECTED",
                          style: TextStyle(
                            color: connected
                                ? Colors.greenAccent
                                : reconnecting
                                    ? Colors.orangeAccent
                                    : Colors.redAccent,
                            fontSize: 22,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                  ),

                  const SizedBox(height: 25),

                  // ===============================
                  // CONNECT BUTTON
                  // ===============================
                  SizedBox(
                    width: double.infinity,
                    height: 55,
                    child: ElevatedButton(
                      onPressed: connected
                          ? null
                          : () {
                              ble.scanAndConnect();
                            },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: const Color(0xFF00D9FF),
                        foregroundColor: Colors.black,
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(18),
                        ),
                      ),
                      child: const Text(
                        "CONNECT",
                        style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          letterSpacing: 2,
                        ),
                      ),
                    ),
                  ),

                  const SizedBox(height: 30),

                  // ===============================
                  // CONTROL BUTTONS
                  // ===============================
                  Row(
                    children: [
                      Expanded(
                        child: SizedBox(
                          height: 65,
                          child: ElevatedButton(
                            onPressed: connected
                                ? () {
                                    ble.move();
                                  }
                                : null,
                            style: ElevatedButton.styleFrom(
                              backgroundColor: Colors.green,
                              foregroundColor: Colors.white,
                              shape: RoundedRectangleBorder(
                                borderRadius: BorderRadius.circular(18),
                              ),
                            ),
                            child: const Text(
                              "MOVE",
                              style: TextStyle(
                                fontSize: 20,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ),
                        ),
                      ),
                      const SizedBox(width: 20),
                      Expanded(
                        child: SizedBox(
                          height: 65,
                          child: ElevatedButton(
                            onPressed: connected
                                ? () {
                                    ble.stop();
                                  }
                                : null,
                            style: ElevatedButton.styleFrom(
                              backgroundColor: Colors.red,
                              foregroundColor: Colors.white,
                              shape: RoundedRectangleBorder(
                                borderRadius: BorderRadius.circular(18),
                              ),
                            ),
                            child: const Text(
                              "STOP",
                              style: TextStyle(
                                fontSize: 20,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ),
                        ),
                      ),
                    ],
                  ),

                  const SizedBox(height: 30),

                  // ===============================
                  // VEHICLE STATE
                  // ===============================
                  Container(
                    width: double.infinity,
                    padding: const EdgeInsets.all(18),
                    decoration: BoxDecoration(
                      color: Colors.white10,
                      borderRadius: BorderRadius.circular(18),
                      border: Border.all(
                        color: const Color(0xFF00D9FF),
                      ),
                    ),
                    child: Column(
                      children: [
                        const Text(
                          "Vehicle State",
                          style: TextStyle(
                            color: Colors.white70,
                            fontSize: 18,
                          ),
                        ),
                        const SizedBox(height: 12),
                        Text(
                          vehicleState,
                          style: const TextStyle(
                            color: Color(0xFF00D9FF),
                            fontSize: 24,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                  ),

                  const SizedBox(height: 20),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'ble_helper.dart';

class SimulationBleService {
  //=========================================================
  // Singleton
  //=========================================================

  static final SimulationBleService instance =
      SimulationBleService._internal();

  SimulationBleService._internal();

  final FlutterReactiveBle _ble = FlutterReactiveBle();

  //=========================================================
  // UUIDs
  //=========================================================

  /// Driver service (advertised)
  static const advertisedServiceUuid =
      "12345678-1234-1234-1234-1234567890AB";

  /// Simulation service (used after connection)
  static const simServiceUuid =
      "12345678-AAAA-BBBB-CCCC-123456789000";

  static const rxUuid =
      "12345678-AAAA-BBBB-CCCC-123456789001";

  static const txUuid =
      "12345678-AAAA-BBBB-CCCC-123456789002";

  String? _deviceId;

  StreamSubscription? _scanSubscription;
  StreamSubscription? _connectionSubscription;
  StreamSubscription? _notifySubscription;

  final ValueNotifier<bool> connectionNotifier =
      ValueNotifier(false);

  bool get isConnected => connectionNotifier.value;

  set isConnected(bool value) {
    connectionNotifier.value = value;
  }

  final ValueNotifier<String> stateNotifier =
      ValueNotifier("Disconnected");

  //=========================================================
  // Scan + Connect
  //=========================================================

  Future<void> scanAndConnect() async {
    final error = await BleHelper.checkBle(_ble);

    if (error != null) {
      stateNotifier.value = error;
      return;
    }

    print("[SIM] Starting Scan");

    stateNotifier.value = "Scanning...";

    _scanSubscription?.cancel();

    _scanSubscription = _ble
        .scanForDevices(
          withServices: [
            Uuid.parse(advertisedServiceUuid),
          ],
          scanMode: ScanMode.lowLatency,
        )
        .listen(
      (device) {

        print("==============================");
        print("[SIM] Device Found");
        print("Name : ${device.name}");
        print("ID   : ${device.id}");
        print("RSSI : ${device.rssi}");
        print("==============================");

        _deviceId = device.id;

        _scanSubscription?.cancel();

        _connect(device.id);

      },
      onError: (e) {

        print("[SIM] Scan Error");

        print(e);

        stateNotifier.value = "Scan Error";

      },
    );
  }

  //=========================================================
  // Connect
  //=========================================================

  Future<void> _connect(String id) async {

    print("[SIM] Connecting...");

    _connectionSubscription?.cancel();

    _connectionSubscription = _ble
        .connectToDevice(
          id: id,
          connectionTimeout: const Duration(seconds: 10),
        )
        .listen(

      (connectionState) {

        switch (connectionState.connectionState) {

          case DeviceConnectionState.connected:

            print("[SIM] Connected");

            isConnected = true;

            stateNotifier.value = "Connected";

            _subscribe();

            break;

          case DeviceConnectionState.connecting:

            print("[SIM] Connecting...");

            stateNotifier.value = "Connecting...";

            break;

          case DeviceConnectionState.disconnected:

            print("[SIM] Disconnected");

            isConnected = false;

            stateNotifier.value = "Disconnected";

            break;

          case DeviceConnectionState.disconnecting:

            print("[SIM] Disconnecting...");

            stateNotifier.value = "Disconnecting...";

            break;
        }

      },

      onError: (e) {

        print("[SIM] Connection Error");

        print(e);

        isConnected = false;

        stateNotifier.value = "Connection Error";

      },

    );
  }

  //=========================================================
  // Subscribe
  //=========================================================

  void _subscribe() {

    if (_deviceId == null) return;

    final characteristic = QualifiedCharacteristic(
      serviceId: Uuid.parse(simServiceUuid),
      characteristicId: Uuid.parse(txUuid),
      deviceId: _deviceId!,
    );

    _notifySubscription?.cancel();

    _notifySubscription =
        _ble.subscribeToCharacteristic(characteristic).listen(

      (data) {

        final message = utf8.decode(data).trim();

        print("[SIM RX] $message");

        if (message.startsWith("STATE:")) {

          stateNotifier.value =
              message.substring(6);

        }

      },

      onError: (e) {

        print("[SIM] Notify Error");

        print(e);

      },

    );
  }
  
    //=========================================================
  // Send Command
  //=========================================================

  Future<void> _send(String command) async {

    if (_deviceId == null) {
      print("[SIM] No device connected.");
      return;
    }

    final characteristic = QualifiedCharacteristic(
      serviceId: Uuid.parse(simServiceUuid),
      characteristicId: Uuid.parse(rxUuid),
      deviceId: _deviceId!,
    );

    try {

      await _ble.writeCharacteristicWithoutResponse(
        characteristic,
        value: utf8.encode(command),
      );

      print("[SIM] Sent: $command");

    } catch (e) {

      print("[SIM] Write Error");

      print(e);

    }
  }

  //=========================================================
  // Public Commands
  //=========================================================

  Future<void> move() async {

    if (!isConnected) {
      print("[SIM] Not connected.");
      return;
    }

    await _send("M");
  }

  Future<void> stop() async {

    if (!isConnected) {
      print("[SIM] Not connected.");
      return;
    }

    await _send("S");
  }

  //=========================================================
  // Disconnect (Optional)
  //=========================================================

  void disconnect() {

    _scanSubscription?.cancel();
    _connectionSubscription?.cancel();
    _notifySubscription?.cancel();

    isConnected = false;

    stateNotifier.value = "Disconnected";
  }

  //=========================================================
  // Dispose
  //=========================================================

  void dispose() {

    disconnect();

    connectionNotifier.dispose();

    stateNotifier.dispose();
  }
}
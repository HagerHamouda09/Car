import 'package:flutter/material.dart';
import 'bluetooth_screen.dart';

class WelcomeScreen extends StatelessWidget {
  const WelcomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        width: double.infinity,
        height: double.infinity,
      decoration: const BoxDecoration(
        gradient: LinearGradient(
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
          colors: [
            Color(0xFF040C12), // Almost black blue
            Color(0xFF07141D), // Dark navy
            Color(0xFF0A1D29), // Blue-teal
            Color(0xFF06131B), // Dark again
          ],
          stops: [0.0, 0.35, 0.75, 1.0],
        ),
      ),
        child: SafeArea(
          child: Center(
            child: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 30),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  // ===========================
                  // LOGO
                  // ===========================
                  Image.asset(
                    'assets/images/logo.png',
                    width: 230,
                  ),

                  const SizedBox(height: 35),

                  // ===========================
                  // TITLE
                  // ===========================
                  const Text(
                    "DRIVER SIMULATION",
                    textAlign: TextAlign.center,
                    style: TextStyle(
                      color: Colors.white,
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 1.5,
                    ),
                  ),

                  const SizedBox(height: 12),

                  // ===========================
                  // SUBTITLE
                  // ===========================
                  const Text(
                    "Guardian Drive Vehicle Simulator",
                    textAlign: TextAlign.center,
                    style: TextStyle(
                      color: Colors.white70,
                      fontSize: 16,
                    ),
                  ),

                  const SizedBox(height: 60),

                  // ===========================
                  // START BUTTON
                  // ===========================
                  SizedBox(
                    width: double.infinity,
                    height: 58,
                    child: ElevatedButton(
onPressed: () {
  Navigator.push(
    context,
    MaterialPageRoute(
      builder: (context) => const BluetoothScreen(),
    ),
  );
},
                      style: ElevatedButton.styleFrom(
                        backgroundColor: const Color(0xFF00D9FF),
                        foregroundColor: Colors.black,
                        elevation: 8,
                        shadowColor: const Color(0x8000D9FF),
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(18),
                        ),
                      ),
                      child: const Text(
                        "START",
                        style: TextStyle(
                          fontSize: 20,
                          fontWeight: FontWeight.bold,
                          letterSpacing: 2,
                        ),
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
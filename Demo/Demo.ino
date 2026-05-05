#include <Wire.h>
#include <MPU6050.h>
#include <ESP32Servo.h>

MPU6050 mpu;
Servo servo;

// ===== PINS =====

// Motor
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13

#define ENA 25
#define ENB 14
#define STBY 32

// Relay
#define RELAY 23

// Sensors
#define HALL_PIN 36
#define TRIG 17
#define ECHO 16

// Buzzer
#define BUZZER 18

// ===== MPU VARIABLES =====
long baseAccel = 0;
bool calibrated = false;

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Relay
  pinMode(RELAY, OUTPUT);

  // Sensors
  pinMode(HALL_PIN, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(BUZZER, OUTPUT);

  // Enable motor driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  // 🔥 TURN ON BATTERY
  digitalWrite(RELAY, HIGH);

  // ===== SERVO SETUP =====
  servo.setPeriodHertz(50);
  servo.attach(19, 500, 2400);
  servo.write(90);   // neutral position

  // I2C + MPU
  Wire.begin(21, 22);
  mpu.initialize();

  Serial.println("Calibrating MPU...");

  delay(1000);

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  baseAccel = sqrt((long)ax*ax + (long)ay*ay + (long)az*az);

  Serial.print("Base Accel: ");
  Serial.println(baseAccel);

  calibrated = true;

  Serial.println("DEMO START");
}

// ===== LOOP =====
void loop() {

  // 🚗 ALWAYS MOVE
  forward();

  // ===== HALL SENSOR =====
  int hall = digitalRead(HALL_PIN);

  if (hall == 0) {
    Serial.println("MAGNET DETECTED");

    stopMotors();
    digitalWrite(RELAY, LOW);

    return;
  } else {
    digitalWrite(RELAY, HIGH);
  }

  // ===== ULTRASONIC =====
  float d = getDistance();

  if (d < 20) {
    Serial.println("OBSTACLE");

    stopMotors();

    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);

    return;
  }

  // ===== CRASH DETECTION =====
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  long accel = sqrt((long)ax*ax + (long)ay*ay + (long)az*az);

  long shock = abs(accel - baseAccel);

  Serial.print("Shock: ");
  Serial.println(shock);

  if (shock > 8000) {
    Serial.println("CRASH DETECTED");

    stopMotors();

    // 🔊 buzzer
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);

    // ===== SERVO PUSH BUTTON =====
    servo.write(30);   // push
    delay(300);

    servo.write(90);   // return

    return;
  }

  delay(100);
}

// ===== MOTOR FUNCTIONS =====

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ===== ULTRASONIC =====

float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);

  if (duration == 0) return 100;

  return duration * 0.034 / 2;
}
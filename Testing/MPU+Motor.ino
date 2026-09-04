#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
bool mpuConnected = false;

// ===== PINS =====
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13

#define ENA 25
#define ENB 14
#define STBY 32

#define RELAY 23
#define BUZZER 4

// ===== CONTROL =====
bool stopped = false;
unsigned long stopTime = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("START");

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Enable motors
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  digitalWrite(RELAY, HIGH);
  digitalWrite(BUZZER, LOW);

  // ===== MPU INIT (FIXED) =====
  Wire.begin(21, 22);
  mpu.initialize();
  delay(100);

  // ❌ skip testConnection (not reliable for MPU6500)
  mpuConnected = true;
  Serial.println("MPU assumed OK");

  forward();
}

void loop() {
  unsigned long now = millis();

  // ===== NORMAL RUN =====
  if (!stopped) {
    forward();
    digitalWrite(RELAY, HIGH);
  }

  // ===== MPU READING =====
  if (mpuConnected) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    Serial.print("AX: "); Serial.print(ax);
    Serial.print(" AY: "); Serial.print(ay);
    Serial.print(" AZ: "); Serial.println(az);

    // Calculate total acceleration
    long accel = sqrt((long)ax * ax + (long)ay * ay + (long)az * az);

    Serial.print("TOTAL: ");
    Serial.println(accel);

    // 🚨 Crash detection
    if (!stopped && accel > 20000) {
      Serial.println("CRASH DETECTED");

      stopped = true;
      stopTime = now;

      stopMotors();
      digitalWrite(RELAY, LOW);
      digitalWrite(BUZZER, HIGH);
    }
  }

  // ===== RESTART =====
  if (stopped && (now - stopTime > 2000)) {
    Serial.println("RESTART");

    stopped = false;
    digitalWrite(RELAY, HIGH);
    digitalWrite(BUZZER, LOW);
    forward();
  }

  delay(5000);
}

// ===== FUNCTIONS =====

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

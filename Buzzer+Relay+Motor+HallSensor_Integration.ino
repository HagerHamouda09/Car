// Buzzer

//===== PIN DEFINITIONS =====
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13

#define ENA 25
#define ENB 14
#define STBY 32

#define RELAY 23
#define HALL_PIN 36
#define BUZZER 4

// Cooldown control
unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 1000; // 1 second lock

bool stopped = false;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(HALL_PIN, INPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  digitalWrite(RELAY, HIGH);
  digitalWrite(BUZZER, LOW);

  forward();
}

void loop() {
  int value = digitalRead(HALL_PIN);
  unsigned long now = millis();

  Serial.println(value);

  // Only trigger if not already stopped AND cooldown passed
  if (value == 0 && !stopped && (now - lastTriggerTime > cooldown)) {

    stopped = true;
    lastTriggerTime = now;

    stopMotors();
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);
  }

  // restart after some time
  if (stopped && (now - lastTriggerTime > 2000)) {
    stopped = false;

    digitalWrite(RELAY, HIGH);
    digitalWrite(BUZZER, LOW);
    forward();
  }
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




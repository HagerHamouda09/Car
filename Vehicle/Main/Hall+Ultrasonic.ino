
Hall & Ultra 

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

#define TRIG 5
#define ECHO 18

// ===== CONTROL =====
unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 1000;

bool stopped = false;

// ===== SETUP =====
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

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  digitalWrite(RELAY, HIGH);
  digitalWrite(BUZZER, LOW);

  forward();
}

// ===== ULTRASONIC FUNCTION =====
int getDistance() {
  long duration;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH, 30000);

  int distance = duration * 0.034 / 2;
  return distance;
}

// ===== LOOP =====
void loop() {
  int hallValue = digitalRead(HALL_PIN);
  int distance = getDistance();
  unsigned long now = millis();

  Serial.print("Hall: ");
  Serial.print(hallValue);
  Serial.print(" | Distance: ");
  Serial.println(distance);

  // ULTRASONIC STOP
  if (distance > 0 && distance < 15 && !stopped) {
    stopped = true;
    lastTriggerTime = now;

    stopMotors();
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);
  }

  //  HALL SENSOR STOP
  if (hallValue == 0 && !stopped && (now - lastTriggerTime > cooldown)) {
    stopped = true;
    lastTriggerTime = now;

    stopMotors();
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);
  }

  // RESTART
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



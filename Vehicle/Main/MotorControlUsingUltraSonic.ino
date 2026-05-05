// ===== MOTOR PINS =====
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13
#define ENA 25
#define ENB 14
#define STBY 32

// ===== ULTRASONIC PINS =====
#define TRIG 4
#define ECHO 5

long duration;
int distance;

void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  // Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(115200);
}

void loop() {
  distance = getDistance();
  Serial.println(distance);

  if (distance > 20) {
    forward();   // safe → move
  } else {
    stopMotors(); // obstacle → STOP
  }

  delay(100);
}

// ===== FUNCTIONS =====

int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}

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

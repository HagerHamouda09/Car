
//HAL with Motors

===== PIN DEFINITIONS =====

// Motor direction pins
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13

// Enable + standby pins
#define ENA 25
#define ENB 14
#define STBY 32

// Hall sensor
#define HALL_PIN 36

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

  // Hall sensor
  pinMode(HALL_PIN, INPUT_PULLUP);

  // Enable motor driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  // Start moving immediately
  forward();
}

void loop() {

    forward();

  int value = digitalRead(HALL_PIN);
  Serial.println(value);

  delay(100);
}

// ===== FUNCTIONS =====

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
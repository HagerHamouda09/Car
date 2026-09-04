// ===== PIN DEFINITIONS =====

// Motor direction pins
#define IN1 26
#define IN2 27
#define IN3 12
#define IN4 13

// Enable + standby pins 
#define ENA 25
#define ENB 14
#define STBY 32   

void setup() {
  // Direction pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Enable pins 
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Activate driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(STBY, HIGH);

  forward();
  stopMotors();
}

void loop() {
  forward();
  delay(3000);

  stopMotors();
  delay(2000);

  backward();
  delay(3000);

  stopMotors();
  delay(3000);
}

//===== FUNCTIONS =====

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
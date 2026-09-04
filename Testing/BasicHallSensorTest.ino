
// Hall sensor

#define HALL_PIN 36

void setup() {
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT_PULLUP);
}

void loop() {
  int value = digitalRead(HALL_PIN);
  Serial.println(value);
  delay(100);
}

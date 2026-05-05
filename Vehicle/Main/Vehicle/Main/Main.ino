void setup() {
  Serial.begin(115200);
  delay(1000); // give time for serial monitor

  Serial.println("ESP32 is running!");
  
  pinMode(2, OUTPUT); // built-in LED (most ESP32 boards)
}

void loop() {
  Serial.println("Loop working...");
  
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
#define RELAY_PIN 23

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 

void loop() {

    digitalWrite(RELAY_PIN, LOW);  
  delay(3000);

  // Start motors
  digitalWrite(RELAY_PIN, HIGH); 

  delay(3000);


}

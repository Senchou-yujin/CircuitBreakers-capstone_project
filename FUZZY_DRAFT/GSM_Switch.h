#define SIM900_POWER_PIN 9  // Pin connected to SIM900's PWRKEY

void setup() {
  Serial.begin(9600);
  pinMode(SIM900_POWER_PIN, OUTPUT);
  digitalWrite(SIM900_POWER_PIN, LOW);  // Initialize low
  Serial.println("SIM900 Power Control Test");
}

void loop() {
  // Test sequence
  powerOnSIM900();
  delay(10000);  // Keep on for 10 seconds
  
  powerOffSIM900();
  delay(5000);   // Stay off for 5 seconds
}

// Turns SIM900 module ON
void powerOnSIM900() {
  Serial.println("Powering ON...");
  digitalWrite(SIM900_POWER_PIN, HIGH);
  delay(1000);  // Required 1-second pulse
  digitalWrite(SIM900_POWER_PIN, LOW);
  
  // Wait for module initialization
  delay(8000);  // Adjust based on your module's boot time
  Serial.println("Module should be ON now");
}

// Turns SIM900 module OFF
void powerOffSIM900() {
  Serial.println("Powering OFF...");
  digitalWrite(SIM900_POWER_PIN, HIGH);
  delay(1000);  // Required 1-second pulse
  digitalWrite(SIM900_POWER_PIN, LOW);
  
  delay(3000);  // Wait for shutdown to complete
  Serial.println("Module should be OFF now");
}
#include <SoftwareSerial.h>

#define SIM900_POWER_PIN 9  // Pin connected to SIM900's PWRKEY
SoftwareSerial mySerial(7, 8);  // SIM900 TX->D7, RX->D8

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(SIM900_POWER_PIN, OUTPUT);
  digitalWrite(SIM900_POWER_PIN, LOW);  // Initialize low
  
  Serial.println("Checking SIM900 power status...");

  // Check if SIM900 responds to AT command
  if (checkSIM900Ready()) {
    Serial.println("SIM900 is ON and responsive.");
  } else {
    Serial.println("SIM900 did not respond. Powering ON...");
    powerOnSIM900();

    // Double-check after power-on
    if (checkSIM900Ready()) {
      Serial.println("SIM900 is now ON and responsive.");
    } else {
      Serial.println("SIM900 still not responding. Check connections.");
    }
  }
}

void loop() {
  // Empty
}

// Turns SIM900 module ON
void powerOnSIM900() {
  Serial.println("Powering ON...");
  digitalWrite(SIM900_POWER_PIN, HIGH);
  delay(1000);  // Required 1-second pulse
  digitalWrite(SIM900_POWER_PIN, LOW);

  delay(8000);  // Wait for module to initialize
  Serial.println("Module should be ON now.");
}

// Check if SIM900 is responsive
bool checkSIM900Ready() {
  mySerial.println("AT");
  unsigned long startTime = millis();
  
  while (millis() - startTime < 2000) {  // Wait up to 2 seconds for a response
    if (mySerial.available()) {
      String response = mySerial.readString();
      Serial.println("Response: " + response);
      if (response.indexOf("OK") != -1) {
        return true;
      }
    }
  }
  return false;
}

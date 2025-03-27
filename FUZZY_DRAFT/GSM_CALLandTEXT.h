#include <SoftwareSerial.h>

#define SIM900_POWER_PIN 9  // Pin connected to SIM900's PWRKEY
// Define GSM module TX & RX pins
SoftwareSerial mySerial(7, 8);

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


// Function to handle missed call and SMS alert
void alertOwner(String number, String message) {
  Serial.println("Calling " + number + "...");
  mySerial.println("ATD" + number + ";");  // Dial the number
  delay(10000);  // Keep call active for 10 seconds (missed call)
  mySerial.println("ATH");  // Hang up the call
  Serial.println("Missed call sent!");

  delay(2000);  // Short delay before sending SMS

  Serial.println("Sending SMS to " + number + "...");
  mySerial.print("AT+CMGS=\"");
  mySerial.print(number);
  mySerial.println("\"");
  delay(500);

  mySerial.print(message);
  delay(500);
  mySerial.write(26); 
  delay(5000);

  Serial.println("SMS alert sent!");
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(SIM900_POWER_PIN, OUTPUT);
  digitalWrite(SIM900_POWER_PIN, LOW);  // Initialize low
  delay(2000);

  Serial.println("Initializing GSM module...");

  // GSM Initialization
  mySerial.println("AT");  // Handshake with GSM module
  delay(500);
  
  mySerial.println("AT+CMGF=1");  // Set text mode
  delay(500);
  mySerial.println("AT+CSCS=\"GSM\"");  // Set character encoding
  delay(500);

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

  Serial.println("GSM module ready!");

  // Test case: Sending alerts based on different conditions
  String ownerNumber = "+639618936396";  // Change this to your number

  // Example Alerts
  alertOwner(ownerNumber, "Test Alert: The fishpond system is operational.");
  delay(10000);  // Delay before the next test alert
  alertOwner(ownerNumber, "Test Warning: Water level is too low! Check the system.");
}

void loop() {
  // Empty loop - Testing only happens in setup()
}

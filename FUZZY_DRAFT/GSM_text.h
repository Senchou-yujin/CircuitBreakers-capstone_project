// D7(RX) = TX(GSM)
// D8(TX) => 20K => RX(GSM) => 10K => GND

#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  delay(2000);  // Wait for module initialization
  
  mySerial.println("AT");  // Check if module is responsive
  delay(500);
  printResponse();
  
  mySerial.println("AT+CMGF=1");  // Set text mode
  delay(500);
  printResponse();
  
  mySerial.println("AT+CSCS=\"GSM\"");  // Set character encoding
  delay(500);
  printResponse();
  
  // Send SMS with exclamation mark
  Serial.println("Sending SMS...");
  mySerial.println("AT+CMGS=\"+639618936396\"");
  delay(500);
  mySerial.print("Hello! :) :D ;) :-P");
  delay(500);
  mySerial.write(26);  // Send Ctrl+Z
  Serial.println("Message sent!");
}

void loop() {
  // Empty
}

void printResponse() {
  while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
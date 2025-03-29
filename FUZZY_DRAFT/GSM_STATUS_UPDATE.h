#include <Fuzzy.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8);  // SIM900 TX->D7, RX->D8
#define LED_PIN 10  // Define LED Indicator

//Mobile Number of the Owner
String ownerNumber = "+639618936396";  // Owner's Number09958919412

bool sentMorning = false;
bool sentAfternoon = false;


RTC_DS3231 rtc;

// Function to blink LED
void blinkLED(int times, int interval) {
    for (int i = 0; i < times; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(interval);
        digitalWrite(LED_PIN, LOW);
        delay(interval);
    }
}

// Function to fade LED in and out
void fadeLED(int times) {
    for (int j = 0; j < times; j++) {
        for (int brightness = 0; brightness <= 255; brightness++) {
            analogWrite(LED_PIN, brightness);  // Fade in
            delay(5);
        }
        for (int brightness = 255; brightness >= 0; brightness--) {
            analogWrite(LED_PIN, brightness);  // Fade out
            delay(5);
        }
    }
}

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
     // Initialize RTC
    Wire.begin();  // Uses hardware I2C (SDA = 20, SCL = 21)
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        blinkLED(3,500);
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set time to compile time
    }

    Serial.println("Initializing GSM module...");
    mySerial.println("AT");
    delay(500);
    if (!mySerial.find("OK")) {  // If "OK" is NOT received
        Serial.println("Error: GSM not responding!");
        blinkLED(4, 500);  // Blink LED 4 times (500ms interval)
        return;
    }
    mySerial.println("AT+CMGF=1");
    delay(500);
    mySerial.println("AT+CSCS=\"GSM\"");
    delay(500);

    Serial.println("GSM module ready!");

}

void loop() {
    float doValue = 3.0;
    float pHValue = 6.0;
    float waterLevelMeters = 1.08;

    // Get current time
    DateTime now = rtc.now();
    int currentHour = now.hour();
    int currentMinute = now.minute();
    
    Serial.println("------------------------------------------------------");
    Serial.print("Hour: "); Serial.print(currentHour);
    Serial.print(" | Minute: "); Serial.print(currentMinute);
    Serial.println("------------------------------------------------------");

    if (currentHour == 13 && currentMinute == 02 && !sentMorning) {
        Serial.println("ALERT: It's Time!");

        Serial.println("Calling +639618936396...");
        mySerial.println("ATD+639618936396;");  // Dial the number
        delay(10000);  // Keep call active for 10 seconds (missed call)
        mySerial.println("ATH");  // Hang up the call
        Serial.println("Missed call sent!");

        delay(2000);  // Short delay before sending SMS

        Serial.println("Sending SMS to +639618936396...");
        mySerial.print("AT+CMGS=\"+639618936396\"\r");
        delay(500);

        mySerial.print("STATUS NG TUBIG\nDO: " + String(doValue) + 
                        "\npH: " + String(pHValue) + 
                        "\nWater Height: " + String(waterLevelMeters) + "m");
        delay(500);
        mySerial.write(26); 
        delay(5000);

        Serial.println("SMS alert sent!");

        sentMorning = true;  // Prevent multiple alerts
    }

    // Reset the flag after 7:00 PM passes
    if (currentHour == 13 && currentMinute > 03) {
        sentMorning = false;
    }

    delay(1000);
}
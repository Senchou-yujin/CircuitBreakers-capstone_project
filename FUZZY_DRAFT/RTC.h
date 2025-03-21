#include <RTClib.h>
#include <SPI.h>
#include "Wire.h"

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("RTC is not Working!");
    while (1);
  }

  //Set Time from the time this code is compiled.
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
}

void loop() {
  // Get Time
  DateTime now = rtc.now();
  char buf2[] = "MM-DD-YYYY-hh:mm:ss";
  String time_str = now.toString(buf2);
  Serial.println(time_str);
}

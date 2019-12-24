#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

void setup() {
  // put your setup code here, to run once:
 Wire.begin();
 rtc.begin();
Serial.begin(115200);
rtc.adjust(DateTime(2019, 12, 20, 13, 48, 0));

}

void loop() {
  // put your main code here, to run repeatedly:

}

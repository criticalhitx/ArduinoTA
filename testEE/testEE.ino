
#include <esp_sleep.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <EEPROM.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define RXD2 16
#define TXD2 17

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  EEPROM.begin(512);
}

void loop() {
  // put your main code here, to run repeatedly:
String x = readKey(10,17);
Serial.println ("Username = "+x);
delay(2000);
}

String readKey (int startadd, int untiladd)
{
  String key;
   for (int i=startadd;i<=untiladd;i++) 
  {
     char karakter = char(EEPROM.read(i));
     if(karakter!=NULL)
        key = String (key+karakter);
     else 
       return key;
     
  }
return key;
}

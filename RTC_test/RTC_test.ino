#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <EEPROM.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

RTC_DS3231 rtc;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

boolean resetState =false;
boolean InterruptTrigger = false;
int TimeStop;
int TimeNow;

void setup() 
{
Serial.begin(9600);

  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  rtc.adjust(DateTime(__DATE__, __TIME__));
  }

EEPROM.begin(512);
pinMode(34, INPUT_PULLUP);
TimeNow = RTCnow();
TimeStop = RTCnext(TimeNow);
Serial.println("nilai Setup :"+String(TimeNow)+" "+String(TimeStop));
//esp_light_sleep_start();
}



void loop()
{
attachInterrupt(digitalPinToInterrupt(34),rusak,RISING);
TimeNow = RTCnow();
TimeStop = RTCnext(TimeNow);
Serial.println(String(TimeStop)+" "+String(TimeNow));
  while((TimeNow<TimeStop) && (!InterruptTrigger))
  {
    int num=0;
    switch(num)
    {
      case 1:
        {
          //
          break;
        }
      case 2://Misalkan ini yang masukin password
      {
        //Read String from smartphone, then compare to the real PIN. then add the logic for resetState if true.
        break;
      }
      default:
      {
        //
      } 
    }
      Serial.println("I am inside the loop, InterruptTriger logic = "+String(InterruptTrigger) +", TimeNow= "+String(TimeNow)+",TimeStop= "+ String(TimeStop));
      DateTime test = rtc.now();
      String detik = String(test.hour())+":"+ String(test.minute())+":"+ String(test.second());
      String tanggal = String(test.day())+"-"+ String(test.month())+"-"+ String(test.year());
      printToOLEDmultisize(detik,tanggal,2,1);
      TimeNow = RTCnow();
      TimeStop = RTCnext(TimeNow);
      delay(1000);
  }

if (!resetState)
{
  writeKeyEEPROM("Omae wa mou shindeiru",32,64);
  Serial.println("Omae wa mou shindeiru yo");
}
else
{
  Serial.println("Anda diloloskan");
}

resetState=false;
InterruptTrigger=false;
delay(2000);
}




int RTCnow()
{
    DateTime now = rtc.now();
    int hasil = (now.year()-2000)*12*31*24 + now.month()*31*24 + now.day()*24 + now.hour();
    return hasil;
}

int RTCnext(int nowRTC)
{
    int waktuMula = (2019-2000)*12*31*24 + 8*31*24 + 25*24 + 0; //176184 ->176352 ->176520
   // Serial.println("BasePoint Hari minggu : " + String(waktuMula));
    DateTime now = rtc.now();
    while(waktuMula<=nowRTC)
    {
      waktuMula+=7*24;
      //Serial.println("Masih jauh bro");
    }
  return waktuMula;
}

void writeKeyEEPROM (String key,int startadd,int untiladd) 
{
  int count = 0;
  for (int i=startadd;i<=untiladd;i++) 
  {
    EEPROM.write(i, key[count]);
    count++;
  }
  EEPROM.commit();
}

String readKey (int startadd, int untiladd)
{
  String key;
   for (int i=startadd;i<=untiladd;i++) 
  {
     char karakter = char(EEPROM.read(i));
     key = String (key+karakter);
  }
return key;
}

void rusak()
{
  //Serial.println("Eh Rusak");
  InterruptTrigger=true;
  resetState=false;
  detachInterrupt(34);
}

void printToOLED (String str)
{
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(str);
  display.display(); 
}

void printToOLEDmultisize (String str1, String str2, int size1 , int size2)
{
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
  display.clearDisplay();
  display.setTextSize(size1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(str1);
  display.setTextSize(size2);
  display.println(str2);
  display.display(); 
}

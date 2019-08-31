#include <esp_sleep.h>
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

boolean x=true;
boolean insertSoon=true;
boolean resetState=false;
boolean interruptTrigger= false;
int TimeNow;
int TimeStop;
String cetak="";
String kalimat="";
boolean enter;
int count;
int count2;
int baterai;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(34,INPUT_PULLUP); //rusak
  pinMode(25,INPUT_PULLUP); // toggle
  pinMode(23,INPUT_PULLUP);//push up button
  pinMode(13,INPUT_PULLUP); // enter
  pinMode(12,INPUT); // batlev
  // put your setup code here, to run once:
  //ThisKeyIsReal!!!ThisKeyIsReal!!!
 // delay(5000);
  Serial.println("Back to Setup");
  if (digitalRead(34)==HIGH)
  {
    Serial.println("SHINDE WITHOUT POWER"); 
   // printToOLED("Maling triggered saat power mati! Swiper jangan mencuri!",1);
    writeKeyEEPROM("Omae wa Mou Shindeiru!",32,64);
  }
 // writeKeyEEPROM("ThisKeyIsReal!!!ThisKeyIsReal!!!",32,64);
  //Kondisi dibawah ini menunjukkan bahwa PIN sudah shindeiru, its printed on the OLED.
    if(EEPROM.read(32)==79)
    {
      printToOLED("Shindeiru Loh",1);
      delay(3000);
      printToOLED("Kagi ga nain desu ga,\nSugu kagi o settei shite kudasai",1);
       String myKey= Serial.readString();
         while(myKey==NULL)
        {
          myKey=Serial.readString();
        }
      writeKeyEEPROM(myKey,32,64);
    }
  writeKeyEEPROM("12345678",10,17);
   
   if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  rtc.adjust(DateTime(__DATE__, __TIME__));}
  TimeNow = RTCnow();
  TimeStop= RTCnext(TimeNow);
 
}

void loop() {
  // put your main code here, to run repeatedly:
 attachInterrupt(34,rusak,RISING);
Serial.println("LewatSiniKah???"); 
Serial.println("BeforeSleepKey: " + readKey(32,64));
delay(200);

  x=digitalRead(25);
  if (x) //Bila PIN 25 rendah, Masuk mode light sleep 
  {
   Serial.println("Mau Tidur zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
     
     delay(100);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
   lightSleep(); 
  }
  else
  {
    Serial.println("Aku Tidak Tertidur");
  }
  
  //While function for active condition [&& pin30==HIGH]

  x=digitalRead(25);
  if (!x) // bila bangun
  {
    TimeNow = RTCnow();
    while(!x && !interruptTrigger && TimeNow<TimeStop-3) // normal time
    {
        
        int num=0;
        switch(num)
        {
          case 1:
            {
              break;
            }
          case 2://Misalkan ini yang masukin password
            {
              //Read String from smartphone, then compare to the real PIN. then add the logic for resetState if true.
              //Send Response to PHONE 
              // Dengan ini tidak mungkin masuk ke while bagian bawah
              break;
            }
          case 3:
            //ini untuk input nilai PIN
          case 4:
           // ini untuk memasukkan nilai kunci.
          default:
            {
              //
            } 
        }
        DateTime test = rtc.now();
        String detik = String(test.hour())+":"+ String(test.minute())+":"+ String(test.second());
        String tanggal = String(test.day())+"-"+ String(test.month())+"-"+ String(test.year());
      //  printToOLEDmultisize(detik,tanggal,2,1);
          String privatekey = readKey(32,64);
          Serial.println("Private Key : "+privatekey);
          printToOLEDtriple(detik,tanggal,privatekey,2,1,1);
          TimeNow=RTCnow();
        x=digitalRead(25);
        Serial.println("Aku didalam While");
        Serial.println("Waktu sekarang : " + String(TimeNow));
        //// kode di bawah Ini hanya untuk tes, tidak mencerminkan realtime
          float batteryLevel = map(analogRead(12), 0.0f, 4095.0f, 0, 100);
          Serial.println(batteryLevel);
          converttoRealTime(176184);
         /////////////////////////////////////////////////////////////////////
        delay(1000);// INI NANTI DIAPUS
    }
  }
  TimeNow = RTCnow();
 
  if((TimeNow<TimeStop)&&!x&&!interruptTrigger) // nah akan masuk fungsi ini bila pin belum dimasukkan via smartphone.
  {
      printToOLED("Insert your PIN before :\n"+converttoRealTime(TimeStop),1);
      boolean isPinTrue = cekPinMasaTenggang();
      while (!isPinTrue && TimeNow<TimeStop)
      {
        isPinTrue=cekPinMasaTenggang();
      }
  } 
/*    if (compare) // compare dengan input pada wallet
      //Auth Succeed, TimeStop+=168, break
    else
      //Auth Fail + Omae wa + TimeStop+=168, break
  }*/
  TimeNow = RTCnow();
  if (TimeStop==TimeNow) //Apakah fungsi ini tidak dibutuhkan lagi? - Bila dalam masa tenggang tidak dimasukkan pin, auto tamat
  {
    //kemudian TimeStop+=168
  }
  
  //Gunakan fungsi IF disini untuk  a)bila TimeNow>=TimeStop ; b) menentukan apakan PIN sama ;||Compare saja 2 alamat di EEPROM, bila tidak sama = shindeiru//
  //If baterai<10 persen, shindeiru

  if (interruptTrigger)
  {
      writeKeyEEPROM("Omae wa mou shindeiru",32,64);
      EEPROM.commit();
  }

Serial.println("Hey Tayo Hey Tayo");
interruptTrigger=false;
delay(1000);

}

void lightSleep()
{
  esp_sleep_enable_timer_wakeup(10000000); // Timer Sleep
  gpio_wakeup_enable(GPIO_NUM_25,GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(GPIO_NUM_34,GPIO_INTR_HIGH_LEVEL);//LOGIKA DIBALIK BIAR GK CAPEK
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  print_wakeup_reason();
}
void rusak()
{
  Serial.println("Shinde");
  interruptTrigger=true;
  detachInterrupt(34);
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    case ESP_SLEEP_WAKEUP_GPIO : Serial.println("Wakeup caused by GPIO"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
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
    }
  return waktuMula;
}

String converttoRealTime(int time) // hanya sampai jam saja.
{
  int year = (time/(12*31*24))+2000;
  int month = (time-((year-2000)*12*31*24))/(31*24);
  int day = (time-((year-2000)*12*31*24)-(month*31*24))/(24);
  int hour = (time-((year-2000)*12*31*24)-(month*31*24) -(day*24));
  return (String(day) + "-" + String(month) + "-" + String(year) +"\n"+ String(hour) + " o'clock");
}

void printToOLED (String str,int size)
{
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
  display.clearDisplay();
  display.setTextSize(size);
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
  display.setTextSize(3);
  display.println("$S-KEY$");
  display.display(); 
}

void printToOLEDtriple (String str1, String str2,String str3, int size1 , int size2, int size3)
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
  display.setTextSize(size3);
  display.println(str3);
  display.display(); 
}

String naikAngka (String angka)
{
  if (angka=="0")
    return "1";
  if (angka=="1")
    return "2";
  if (angka=="2")
    return "3";
  if (angka=="3")
    return "4";
  if (angka=="4")
    return "5";
  if (angka=="5")
    return "6";
  if (angka=="6")
    return "7";
  if (angka=="7")
    return "8";
  if (angka=="8")
    return "9";
  if (angka=="9")
    return "0";
}

void tambah ()
{
  Serial.println("triggered");
  cetak=naikAngka(cetak);
  detachInterrupt(23);
  //delay(300);
  enter=true;
}

void pindahdigit ()
{
 Serial.println("triggert ganti digit");
 count2++;
 kalimat+=cetak;
 delay(100);
 detachInterrupt(13);
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

void writeKeyEEPROMnocommit (String key,int startadd,int untiladd) 
{
  int count = 0;
  for (int i=startadd;i<=untiladd;i++) 
  {
    EEPROM.write(i, key[count]);
    count++;
  }
  //EEPROM.commit();
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

boolean cekPinMasaTenggang ()
{
      count2=0;
      cetak="0";
       kalimat="";
       enter=false;
             while(count2<8)
              {
               attachInterrupt(23,tambah,RISING);
               attachInterrupt(13,pindahdigit,RISING);
                 while(!enter && count2<8) //Saat enter keluar, loop sudah true
                 {
                  delay(500);
                 }
                 if(count2==8)
                    {
                          printToOLEDmultisize("PIN SALAH","Omae wa mou shindeiru",2,1);
                         break;
                    }
               
               Serial.println("Keluar dari enter");
               enter=false;
               printToOLEDmultisize(kalimat+cetak,"Enter PIN 8 digit",2,1);
               
              }
             Serial.println("Pin yang anda masukkan= " +kalimat);
             if(kalimat==readKey(10,17))
               {TimeStop+=168;
                printToOLEDmultisize("SUCCEED","New Deadline:\n "+converttoRealTime(TimeStop),1,1);
               delay(3000);
               return true;}
             else
               {printToOLEDmultisize("FAIL","Please Try Again",1,1);
               delay(3000);
               return false;}
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* #define ED25519_DLL */
#include "src/ed25519.h"
#include "src/ge.h"
#include "src/sc.h"
//------------------------
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
#include <Crypto.h>
#include <Curve25519.h>
#include <RNG.h>
#include <string.h>

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
String nilaiString;
String readString;
boolean enter;
int count;
int var=0;
int count2;
int baterai;
int NextAuth;
unsigned char seeds[32];

//------------------------------------- START OF SHA DECLARATION-----------------------------------------------//
char hex[256];
uint8_t data[256];
int start = 0;
int seconds = 0;
uint8_t hash[32];
String pin;
#define SHA256_BLOCK_SIZE 32

typedef struct {
  uint8_t data[64];
  uint32_t datalen;
  unsigned long long bitlen;
  uint32_t state[8];
} SHA256_CTX;


#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
//------------------------------------- END OF SHA DECLARATION ----------------------------------------------------//


// 32-63 PrivateKey, 10-17 PIN, 18-23 TimeStop, 24-29 NextAuth //65-77
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  EEPROM.begin(512);
  RNG.begin("TestCurve25519 1.0");
  
  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  rtc.adjust(DateTime(__DATE__,__TIME__));}
  
  pinMode(34,INPUT_PULLUP); //rusak
 // pinMode(25,INPUT_PULLUP); // toggle
  pinMode(23,INPUT_PULLUP);//push up button
  pinMode(13,INPUT_PULLUP); // enter
  pinMode(12,INPUT); // batlev
  // to return back the time
  //writeKeyEEPROM("100000",24,29);
  //-----------------------------------
  Serial.println("Back to Setup");
  
   //-----------Kondisi Bongkar tanpa Power---------------
   if (digitalRead(34)==HIGH)
  {
    writeKeyEEPROM("Omae wa Mou Shindeiru!",32,64);
    writeKeyEEPROM("qwertyuiopasd",65,77);
  }
  //-------------------------------------------------------


 //writeKeyEEPROM("aiharakotoko",65,77);

 
  //Kondisi dibawah ini menunjukkan bahwa PIN sudah shindeiru, its printed on the OLED.  {DEPRECATED API}
    /*if(EEPROM.read(32)==79)//EEPROM.read(32)==79 || EEPROM.read(32)==66
    { 
      printToOLED("Shindeiru Loh",1);
      delay(3000);
      printToOLED("Kagi ga nain desu ga,\nSugu kagi o settei shite kudasai",1);
       String myKey= Serial2.readString();
         while(myKey!="4") // masuk menu insert key di aplikasi
        {
          myKey=Serial2.readString();
        }
        String myKeyShin = Serial2.readString();
        {
          while(myKeyShin==NULL)
          {
            myKeyShin=Serial2.readString();
          }
        }
     // Serial.println("Done Reading");
      writeKeyEEPROM(myKeyShin,32,64);
    }*/
    
 //writeKeyEEPROM("12345678",10,17); 
//  writeKeyEEPROM("176184",24,29); // which is hari minggu lalu (176184) //This line will be deleted. we can fill this space using another scetch.
}

void loop() {
  // put your main code here, to run repeatedly:
  TimeNow = RTCnow();
  TimeStop= RTCnext(TimeNow);
  Serial.println("TimeNow = "+String(TimeNow));//
  Serial.println("TimeStop = "+String(TimeStop));//
  writeKeyEEPROM(String(TimeStop),18,23);
  NextAuth=stringToInt(readKey(24,29)); // next auth 24 to 29
  Serial.println("NextAuth = "+String(NextAuth));//

  if (TimeNow>=NextAuth) // User Ignored Authentication
  {
    flushMem();
  }

  ///////////////////////////////////////////////////////////////////////////////////
attachInterrupt(34,rusak,RISING);
Serial.println("LewatSiniKah???"); 
Serial.println("BeforeSleepKey: " + readKey(32,64));
delay(200);

 /* x=digitalRead(25);
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
  }*/
  
  //While function for active condition [&& pin30==HIGH]

//  x=digitalRead(25);

  x=false;
  if (!x) // bila bangun
  { 
    while(!x && !interruptTrigger)  // normal time
    {
      //---------- Update variable waktu di awal ----------------
      TimeNow = RTCnow();
      TimeStop = RTCnext(TimeNow);
      String NextAuthw = readKey(24,29);
      NextAuth = stringToInt(NextAuthw);
      //---------------------------------------------------------
       
       // ----------------Bila Bongkar-----------------------------------------------------
        if (interruptTrigger) //Menu Bongkar --->> flush username juga
        {
          flushMem();
        }
       // ---------------------------------------------------------------
  
       //------------ Masa Tenggang---------------------------------------------------------
        if((TimeNow<NextAuth)&&(TimeNow>=NextAuth-1)) // nah akan masuk fungsi ini bila pin belum dimasukkan via smartphone.
        {
            boolean cox = ShindeiruKa();
            if(!cox)
            {
            printToOLED("Insert your PIN\nbefore \n\n Sunday 12 AM",1);
            boolean isPinTrue = cekPinMasaTenggang();
              while ((!isPinTrue)&&(!interruptTrigger))
              {
                isPinTrue=cekPinMasaTenggang();
              }
            }
            else
            {
              printToOLED("WALLET UNUSABLE",1);
              delay(400);
            }
        }
        //--------------------------------------------------------------------------------------
        var=0;
        readString="";
        while (Serial2.available()){ 
              char c= Serial2.read();
              readString += c;}
        
        if (readString.length() > 0)   {  
        var = readString.toInt();}
        
        boolean shindekah = ShindeiruKa();

        /// EDIT THIS FOR PROGRAMING DEBUGGING---
        
        //---------- Shindeiru bila telat masuk ----------- 
         if (TimeNow<NextAuth)
        {
          
        }
        else
        {
          flushMem();
        }
        //-----------------------
        /// Penentuan mode perusakan kunci-------
        if(shindekah) 
        {
          var=2;
        }
   
        /// --------------------------------------
        
        //Start of the looping state
        switch(var)
        {
          case 1: // Shindeiru Toki -> Saat terlambat masukin pin 1 minggu , maka di menu ini harus disertakan dengan NextAuth yang diupdate senilai TimeStop
            {
              printToOLED("**Shindeiru Toki Mode**\n\nYou forgot to enter PIN",1);
              delay(1500);
              String respCodeShindetoki = waitfromHPshindeirutoki(); // nunggu string 4,3,219,8
              if (respCodeShindetoki=="219")
              {
                String unameChk = readKey(65,77);
                Serial2.print(unameChk); // kirim username ke HP untuk dicocokkan
                String pkey= queryfromHPwithmessage("Verify the secret key first, then insert it");
                if (pkey!="moemoe")
                {
                  printToOLED("VALID",1);
                  delay(1000);
                  writeKeyEEPROM(pkey,32,63);
                }
              }
              else if (respCodeShindetoki=="3") // Register mode saat 
              {
                modeRegister();
              }
              else if(respCodeShindetoki=="8")
              {
                modeRestoreSK();
              }
              else if(respCodeShindetoki=="4")
              {
                modeLogin();
                //-------------- Must Enter Shindeiru Toki ------------- //
                printToOLED("**Shindeiru Toki Mode**\nU forgot to enter PIN",1);
                  waitfromHPwithmessage("219","Now please go to Shindeiru Toki Mode"); // nunggu string 219
                  String unameChk = readKey(65,77);
                  Serial2.print(unameChk); // kirim username ke HP untuk dicocokkan
                  String pkey= queryfromHPwithmessage("Verify the secret key first, then insert it");
                  if (pkey!="moemoe")
                  {
                    writeKeyEEPROM(pkey,32,63);
                    printToOLED("Recovered",1);
                    delay(1000);
                  }
              }
 
 //!!!!!!!!!!! IMPORTANT : not only waiting 219, but also waiting for login. Selanjutnya if 219, lewatkan saja , atau bila 4, menunggu informasi login benar baru dipass. 
// Selanjutnya adalah operasi untuk query from HP Secret Key.
              if(EEPROM.read(32)!=79) // Bila tidak shindeiru lagi, update variabel nextStop
              {
                TimeNow = RTCnow();
                TimeStop= RTCnext(TimeNow);
                writeKeyEEPROM(String(TimeStop),24,29);
                delay(1000);
              }
              
              waitformoemoe();
              var=0;
              readString="";
              delay(1000);
              break;
            }
          case 2://Mode Bongkar
            {
                String respCodeBongkar = waitfromHPbongkar(); // nunggu string 219,8
                if (respCodeBongkar=="3") // Register mode saat 
                {
                  modeRegister();
                }
                else if(respCodeBongkar=="8")
                {
                  modeRestoreSK();
                  waitformoemoe();
                }
            
             if(EEPROM.read(32)!=79) // Bila tidak shindeiru lagi, update variabel interuptTrigger
              {
                TimeNow = RTCnow();
                TimeStop= RTCnext(TimeNow);
                writeKeyEEPROM(String(TimeStop),24,29);
                interruptTrigger=false;
              }
                
                
                var=0;
                readString="";
                break;
              // Flush semua isinnya
              // Bisa menunggu mode Register and RecoverSK
            }
          case 3: //ModeRegister
            {
              modeRegister();
              break;
            }
          case 4: // Mode Login , user cek dulu ke wallet kesamaan username, bila ok kirim string OK. pada hp dengan handler, bila OK, cek ke server
            {
              modeLogin();
              break;
            }
            //
          case 5: //ini untuk input Change PIN
           {
            printToOLED("**CHANGE PIN MODE**\n\nPlease enter your old PIN",1);
     
            String pinfromHP= queryfromHP();//pin dr hp
              if(pinfromHP!="moemoe")
                {
                  String pinAwal = readKey(10,17);
                    if (pinAwal==pinfromHP) // Bila pin di wallet ama old pin sama,
                    {
                      printToOLED("PIN verified!!\n\nNow enter new PIN!",1); 
                      Serial2.print("OK");
                      String newPIN  = queryfromHP();
                      if(newPIN!="moemoe") // klo moemoe keluar aj
                      {
                        writeKeyEEPROM(newPIN,10,17);
                        printToOLED("Success changing PIN",1);
                        delay(1300);
                      }
                      else
                      {
                        break;
                      }
                    }
                  else
                  {
                    printToOLED("PIN BEDA",1);
                  }
                  waitformoemoe(); // 
                }    
            readString=""; // IF YOU WONDER WHY AFTER EXIT, VAR NOT CHANGE
            var=0;
            break;
           }
           
          case 6: // Menu Receive
          {
            printToOLED("Receive Mode\n\nPlease enter your PIN!",1);
            //---------- Buat Baca PIN ---------------------------------
            String pinAsli = readKey(10,17);
            String pinfromHP= queryfromHP();//pin dr hp
            printToOLED("Diterima = " +pinfromHP,1);
            //------------------------------------------
            // -------- Buat Cek PIN -------------
            if(pinAsli==pinfromHP)
            {
              printToOLED("PIN BENAR",1);
              String PubKey = readPubKey();
              String r = randStr(64); // Generate Random
              //r is 64 bit
              String val =SHA256(r+PubKey);
              String uname = readKey(65,77);
              String send6 = r + "#" + uname; //random string + username
              Serial2.print(send6);
              
              printToOLED("Generated Stealth Address:\n"+val,1);
              waitformoemoenooled();
              
            }
            else
            {
              printToOLED("PIN SALAH",1);
              Serial2.print("Rejected");
              waitformoemoe();
            }
            
            
            readString="";
            var=0;
            break;
          }
          case 7: //Menu Cek Balance
          {
            printToOLED("**Cek Balance Mode**",1);
            delay(1000);
            
            String pinAsli = readKey(10,17);
            String pinfromHP= queryfromHP();//pin dr hp
            printToOLED("Diterima = " +pinfromHP,1);
            //------------------------------------------
            // -------- Buat Cek PIN -------------
            if(pinAsli==pinfromHP)
            {
              printToOLED("PIN BENAR",1);
              String unameMode7 = readKey(65,77); // Bila benar kirim username.
              Serial2.print(unameMode7); // Kirim ke HP
              delay(1000);
            }
            else
            {
              printToOLED("PIN SALAH",1);
              Serial2.print("Rejected");
            }

            
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }

          case 8: //Menu RestoreSK
          {
            modeRestoreSK();
            interruptTrigger=false;
                TimeNow = RTCnow();
                TimeStop= RTCnext(TimeNow);
                writeKeyEEPROM(String(TimeStop),24,29);
            waitformoemoenooled();
            break;
          }

          //Option 1 : Untuk menu Send, PIN dikirimkan ke HP untuk dicocokkan. Wallet menunggu jawaban "OK". pencocokan dilakukan di HP
          //           bila PIN SAMA { tvResponse di NULL kan, HP kirim string "OK" ke wallet } , baru lah wallet mengirimkan kembali username.
          //           yang kemudian listenernya merespon. 
          //Option 2 : PIN di awal, pertama wallet menunggu input pin dahulu, bila sudah kirim ke HP ,wallet melakukan pengecekan,  Bila sama, kirim username ke HP.           
          
          case 9: //Menu Send
          {
            printToOLED("**Send Mode**\nPlease enter your PIN to enter this mode!!",1);
            delay(1000);
            // Menunggu PIN DARI HP--------- /////
            String pinAsli = readKey(10,17);
            String pinfromHP= queryfromHP();//pin dr hp
            if(pinAsli==pinfromHP)
            {
              printToOLED("PIN BENAR",1);
              String unameMode9 = readKey(65,77);
              Serial2.print(unameMode9); // Kirim username ke HP
            }
            else
            {
              printToOLED("PIN SALAH",1);
              Serial2.print("Rejected");
            }
            ///----------------------------- //////
            
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }
          case 11: //HistoryIn Mode
          {
            printToOLED("**History In Mode**\n\nPlease enter your PIN to enter this mode!!",1);
            Serial2.print("Connected Broh");
            delay(1000);
            
            String pinAsli = readKey(10,17);
            String pinfromHP= queryfromHP();//pin dr hp
            if(pinAsli==pinfromHP)
            {
              printToOLED("PIN BENAR",1);
              String unameMode11 = readKey(65,77);
              Serial2.print(unameMode11); // Kirim username ke HP
            }
            else
            {
              printToOLED("PIN SALAH",1);
              Serial2.print("Rejected");
            }

            
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }
          case 12:
            {
            printToOLED("**History Out Mode**\n\nPlease enter your PIN to enter this mode!!",1);
            delay(1000);
            // Masukkan PIN dri HP
            String pinAsli = readKey(10,17);
            String pinfromHP= queryfromHP();//pin dr hp
            if(pinAsli==pinfromHP)
            {
              printToOLED("PIN BENAR",1);
              String unameMode12 = readKey(65,77);
              Serial2.print(unameMode12); // Kirim username ke HP
            }
            else
            {
              printToOLED("PIN SALAH",1);
              Serial2.print("Rejected");
            }

            
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }
 
          default:
            {
              //
            } 
        }
        DateTime test = rtc.now();
        String detik = String(test.hour())+":"+ String(test.minute())+":"+ String(test.second());
        String tanggal = String(test.day())+"-"+ String(test.month())+"-"+ String(test.year());
        
          /// ----- Code Below is to Set Main Menu Mode on OLED ----------
          String privatekey = readPrivKey();
          String unamex = readKey(65,77);
          //Serial.println("Private Key : "+privatekey);
          printToOLEDtriple(detik,tanggal,"\nS-KEY",2,1,1);
          TimeNow=RTCnow();
          TimeStop=RTCnext(TimeNow);
          String uname=readKey(65,77);

          delay(700);
          printToOLEDquint(String(TimeNow),String(TimeStop),readKey(24,29),readKey(65,77),readKey(10,17),1,1,1,1,1);
          //delay(700);
         // ----------------Bila Bongkar-----------------------------------------------------
        if (interruptTrigger) //Menu Bongkar --->> flush username juga
        {
            writeKeyEEPROM("Omae wa mou shindeiru",32,63); //Flush SecretKey
            writeKeyEEPROM("qwertyuiopasd",65,77); // Flush Username
            String x = randStr(8);
          //  writeKeyEEPROM(x,10,17); // Flush PIN!! PLEASE ENABLE THIS IN REAL ENVIRONMENT
            EEPROM.commit();
        }
       // ----------------------------------------------------------------
          delay(1000);
        
        //// kode di bawah Ini hanya untuk tes, tidak mencerminkan realtime
         /////////////////////////////////////////////////////////////////////

    }
  }
// ----------------Bila Bongkar-----------------------------------------------------
if (interruptTrigger) //Menu Bongkar --->> flush username juga
{
  flushMem();
}
// ----------------------------------------------------------------
interruptTrigger=false;

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

void flushMem()
{
    writeKeyEEPROM("Omae wa Mou Shindeiru!!",32,64); //flush private Key
    writeKeyEEPROM("Omea wa Mou Shindeiru!!",100,131); // flush public Key
    writeKeyEEPROM("qwertyuiopasd",65,77); // Flush Username
    String x = randStr(8);
    writeKeyEEPROM(x,10,17); // Flush PIN!! PLEASE ENABLE THIS IN REAL ENVIRONMENT
    EEPROM.commit();
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
    //DateTime now = rtc.now();
    //int hasil = (now.year()-2000)*11*30*24 + (now.month()-1)*30*24 + (now.day()-1)*24 + now.hour(); // Tidak ada tanggal 0 dan bulan 0
//--------------------------------------BEGINNING OF TIMIMG SECTION ------------------------------//
int RTCnow()
{
  DateTime now = rtc.now();
  int hari=now.day();
  int bulan=now.month();
  int tahun=now.year();
  
  int YTDcount = DayYTD(hari,bulan,tahun);
  int TahunBerlalu= DayLewat(tahun);
  int YTDtotal = YTDcount+TahunBerlalu;
  return YTDtotal;
}

int DayYTD(int hr,int bln,int th)
{
  boolean kab= isKabisat(th);
  int febDay;
  if(kab)
    febDay=29;
  else
    febDay=28;
  //-----------------------------//
  if(bln==1)
    return hr;
  else if(bln==2)
    return 31+hr;
  else if(bln==3)
    return 31+febDay+hr;
  else if(bln==4)
    return 31+febDay+31+hr;
  else if(bln==5)
    return 31+febDay+31+30+hr;
  else if(bln==6)
    return 31+febDay+31+30+31+hr;
  else if(bln==7)
    return 31+febDay+31+30+31+30+hr;
  else if(bln==8)
    return 31+febDay+31+30+31+30+31+hr;
  else if(bln==9)
    return 31+febDay+31+30+31+30+31+31+hr;
  else if(bln==10)
    return 31+febDay+31+30+31+30+31+31+30+hr;
  else if(bln==11)
    return 31+febDay+31+30+31+30+31+31+30+31+hr;
  else if(bln==12)
    return 31+febDay+31+30+31+30+31+31+30+31+30+hr;
}

int DayLewat(int th)
{
  int count = 100000;
  for (int i=2019;i<th;i++)
  {
    if(isKabisat(i))
      count+=366;
    else
      count+=365;
  }
  return count;
}

boolean isKabisat(int th)
{
  if(th%4==0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int RTCnext(int rtcnow)
{
  int x=100335; // Nilai awal
  while(x<=rtcnow)
  {
    x+=7;
  }
  return x;
}

//-----------------------------END OF TIMING SECTION---------------------------------------------------//

String converttoRealTime(int time) // hanya sampai jam saja.
{
  int year = (time/(11*31*24))+2000;
  int month = ((time-((year-2000)*11*31*24))/(31*24))+1;
  int day = (time-((year-2000)*11*31*24)-(month*31*24))/(24);
  int hour = (time-((year-2000)*11*31*24)-(month*31*24) -(day*24));
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

void printToOLEDquad (String str1, String str2,String str3,String str4, int size1 , int size2, int size3, int size4)
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
  display.setTextSize(size4);
  display.println(str4);
  display.display(); 
}

void printToOLEDquint (String str1, String str2,String str3,String str4,String str5, int size1 , int size2, int size3, int size4,int size5)
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
  display.setTextSize(size4);
  display.println(str4);
  display.setTextSize(size5);
  display.println(str5);
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
     if(karakter!=NULL)
        key = String (key+karakter);
     else 
       return key;
     
  }
return key;
}

boolean cekPinMasaTenggang ()
{
      count2=0;
      cetak="0";
       kalimat="";
       enter=false;
             while((count2<8)&&(!interruptTrigger))
              {
               attachInterrupt(23,tambah,RISING);
               attachInterrupt(13,pindahdigit,RISING);
                 while((!enter) && (count2<8) && (!interruptTrigger)) //Saat enter keluar, loop sudah true
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
               {TimeStop+=7;
                writeKeyEEPROM(String(TimeStop),24,29);
                printToOLEDmultisize("SUCCEED\n","New Deadline :\nSunday Next Week",1,1);
               delay(2000);
               return true;}
             else
               {printToOLEDmultisize("FAIL\n","Please Try Again",1,1);
               delay(2000);
               return false;}
}

int stringToInt(String x)
{
  int key=0;
  int charint;
  for (int i=0;i<=5;i++)
  {
    char nilaiperchar = x[i];
      if(nilaiperchar=='0')
        charint=0;
      if(nilaiperchar=='1')
        charint=1;
      if(nilaiperchar=='2')
        charint=2;
      if(nilaiperchar=='3')
        charint=3;
      if(nilaiperchar=='4')
        charint=4;
      if(nilaiperchar=='5')
        charint=5;
      if(nilaiperchar=='6')
        charint=6;
      if(nilaiperchar=='7')
        charint=7;
      if(nilaiperchar=='8')
        charint=8;
      if(nilaiperchar=='9')
        charint=9;
      
    int perkalian = 1;
      for (int ii=5;ii>i;ii--)
      {
        perkalian*=10;
      }
    key+=(charint*perkalian);
  }
  return key;
}


boolean ShindeiruKa()
{
  if(EEPROM.read(32)==79)
    return true;
  else 
    return false;
}

void waitfromHP (String desiredValue) //Wait until phone send desidedvalue
{
   String query = Serial2.readString(); // disuruh query
   printToOLED("Now Waiting..." ,1);
   delay(500);
    while((query!=desiredValue)&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLED("Now Waiting...",1);
      delay(500);
    }
}

void waitfromHPwithmessage (String desiredValue, String message) //Wait until phone send desidedvalue
{
   String query = Serial2.readString(); // disuruh query
   printToOLED(message,1);
   delay(500);
    while((query!=desiredValue)&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLED(message,1);
      delay(500);
    }
}

void waitfromHPmodified (String desiredValue, String message) //Wait until phone send desidedvalue
{
   String query = Serial2.readString(); // disuruh query
   printToOLED(message,1);
   delay(500);
    while(query!=desiredValue&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLED(message,1);
      delay(500);
    }
}

String waitfromHPbongkar() //Pada mode bongkar, menunggu angka 219,3, dan 8 
{
   String query = Serial2.readString(); // disuruh query
   printToOLEDquad("Your wallet is broken!!","Option Available:","*Register new Wallet","*Recover w/ Secret Key",1,1,1,1);
   delay(1000);
    while((query!="219")&&(query!="3")&&(query!="8")&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
      delay(500);
    }
    return query;
}

String waitfromHPshindeirutoki() //Pada mode Shindeiru Toki, menunggu angka 4,219,3,dan 8
{
   String query = Serial2.readString(); // disuruh query
   printToOLED("Please go to Shindeiru Toki to recover\n\nOther options:\nRegister, RecoverSK" ,1);
   delay(1000);
    while((query!="219")&&(query!="3")&&(query!="8")&&(query!="4")&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
      printToOLED("Please go to Shindeiru Toki to recover\n\nOther options:\nRegister, RecoverSK" ,1);
      delay(500);
    }
    return query;
}


void waitformoemoe() //Wait until phone send moemoe
{
   String query = Serial2.readString(); // disuruh query
   printToOLEDmultisize("Do your task first,\n","Then press the logo button to exit",1,1);
   delay(500);
    while(query!="moemoe"&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLEDmultisize("Do your task first,\n","Then press the logo button to exit",1,1);
      delay(500);
    }
}

void waitformoemoenooled() //Wait until phone send moemoe
{
   String query = Serial2.readString(); // disuruh query
   delay(500);
    while((query!="moemoe")&&(!interruptTrigger)) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
      delay(500);
    }
}

String queryfromHP() // Menungu respon dari HP
{
    String nilaiString;
    while((nilaiString==NULL)&&(!interruptTrigger))
    {
      while((Serial2.available())&&(!interruptTrigger))
      {
        delay(500);
        nilaiString=Serial2.readString();
      }
      
    }
    return nilaiString;
}

String queryfromHPwithmessage(String message) // Menungu respon dari HP
{
    printToOLED(message,1);
    String nilaiString;
    while((nilaiString==NULL)&&(!interruptTrigger))
    {
      while((Serial2.available())&&(!interruptTrigger))
      {
        delay(500);
        nilaiString=Serial2.readString();
      }
    }
    return nilaiString;
}



String randStr (int numBytes) // Generate random number fixed size
{
  int i = 0;
  String letters[37] = {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
  String randString;
    for(i = 0; i<numBytes; i++)
    {
     randString+=letters[random(0, 36)];
    }
    return randString;
}

void modeRegister() // Mode3
{
    printToOLED("Register Mode",1); //Welcome message
    delay(1000);
    String pubKey = GenerateAndPutPrivateKeyAndReturnPublicKey(); //Create and Write Private and Pub key and store to EEPROM
    String pkNew = readPrivKey();
    printToOLED("Private Key = "+ pkNew,1);
    Serial2.print(pubKey);
    
    //--------------------
    writeKeyEEPROM("12345678",10,17); // Default reset PIN when registering
    String newuname = queryfromHP();   // query username dari user
    writeKeyEEPROM(newuname,65,77);
    
    printToOLEDmultisize("Register Success\n",pkNew,1,1);
    waitformoemoenooled();
             
              interruptTrigger=false;
              TimeNow = RTCnow();
              TimeStop= RTCnext(TimeNow);
              writeKeyEEPROM(String(TimeStop),24,29);
    
    var=0;
    readString="";
    delay(1000);
}

String GenerateAndPutPrivateKeyAndReturnPublicKey()
{
    unsigned char nonce[32], private_key[32];
    
    /* create a random seed, and a keypair out of that seed */
    getSeeds();
    ed25519_create_keypair(nonce, private_key, seeds);
    
    String hasil;
    //Get the private key value //
    for (int j=0;j<32;j++)
    {
      String karakter= String(private_key[j],HEX);
      int leng = karakter.length();
      if(leng==1)
      {
        karakter="0"+karakter;
      }
      hasil+=karakter;
    }
    Serial.println("Hasil private_key = "+hasil);
    
    //--- Put Generated Private Key into memory ------------
     for(int i=32;i<=63;i++)
    {
      EEPROM.write(i,private_key[i-32]);
      EEPROM.commit();
    }
    //--- Generate the public key from private key
    
    // Evaluate the curve function and check the public keys.
    uint8_t result[32];
    Serial.flush();
    unsigned long start = micros();
    Curve25519::eval(result, private_key, 0);

    //--- Put Generated Public Key into memory ------------
     for(int i=100;i<=131;i++)
    {
      EEPROM.write(i,result[i-100]);
      EEPROM.commit();
    }

    String myPublicKey = readPubKey();
    return myPublicKey;
}

void getSeeds()
{
  ed25519_create_seed(seeds);
  for(int i=0;i<=31;i++)
  {
    int x = random(256);
    seeds[i]=x;
  }
}

String readPrivKey() //Read Hex format into STR ( Private Key )
{
    String key;
    for(int i=32;i<=63;i++)
    {
       String karakter = String(EEPROM.read(i),HEX);
       if (karakter.length()==1)
       {
        karakter="0"+karakter;
       }
       if(karakter!=NULL)
          key = String (key+karakter);
       else 
         return key;
    }
}

String readPubKey() //Read Hex format into STR ( Public Key )
{
    String key;
    for(int i=100;i<=131;i++)
    {
       String karakter = String(EEPROM.read(i),HEX);
       if (karakter.length()==1)
       {
        karakter="0"+karakter;
       }
       if(karakter!=NULL)
          key = String (key+karakter);
       else 
         return key;
    }
}

String readPubKeyTemp() //Read Hex format into STR ( Public Key Temp untuk menu restore)
{
    String key;
    for(int i=200;i<=231;i++)
    {
       String karakter = String(EEPROM.read(i),HEX);
       if (karakter.length()==1)
       {
        karakter="0"+karakter;
       }
       if(karakter!=NULL)
          key = String (key+karakter);
       else 
         return key;
    }
}

void modeRestoreSK() // Mode 8
{
    printToOLED("**Mode Restore with SecretKey**\n\nPlease Click Proceed",1);
    String SecKey = queryfromHP(); // Get Secret Key to Server
    
    if(isValidRSK(SecKey))
    {
      unsigned char SKConvertedToHexArray[32];
      for(int i=0;i<=31;i++)
      {
        String temp;
        char temp1=SecKey[2*i];//MSB
        char temp2=SecKey[(2*i)+1];//LSB
        int valtemp1=valueOfHex(temp1);
        int valtemp2=valueOfHex(temp2);
        int nilai = valtemp1*16+valtemp2;
        SKConvertedToHexArray[i]=nilai;
      }
      
        // Evaluate the curve function and check the public keys.
        uint8_t result[32];
        Serial.flush();
        unsigned long start = micros();
        Curve25519::eval(result, SKConvertedToHexArray, 0);

      //--- Put Generated Public Key into memory for temp ------------
       for(int i=200;i<=231;i++)
      {
        EEPROM.write(i,result[i-200]);
        EEPROM.commit();
      }

      String insertedPubKey=readPubKeyTemp(); //Read TempPubKey // PubKey hasil Curve25519 dari PrivKey yang dimasukkan
      printToOLED("Please Wait ..",1);
      String verif = SHA256(insertedPubKey);
      Serial2.print(verif);//Kirim Ke HP Public Key untuk dicocokkan

      String confirmMessage = queryfromHP();
      
      if(confirmMessage=="Beda")
      {
        printToOLED("Key Not Found",1);
        delay(3000);
      }
      else if(confirmMessage=="moemoe")
      {
        
      }
      else
      {
        //--- Put Private Key into memory ------------
         for(int i=32;i<=63;i++)
        {
          EEPROM.write(i,SKConvertedToHexArray[i-32]);
          EEPROM.commit();
        }
        //--- Put Public Key into memory ------------
         for(int i=100;i<=131;i++)
        {
          EEPROM.write(i,result[i-100]);
          EEPROM.commit();
        }
        //Put username also
        writeKeyEEPROM(confirmMessage,65,77);
        printToOLED("Recover Success\n\nPress Logo to Exit",1);
      }
    }
    else
    {
      printToOLED("Private Key Format not Valid",1);
      delay(1000);
    }
    
    writeKeyEEPROM("12345678",10,17);
    readString="";
    var=0;
}

boolean isValidRSK(String key)
{
  boolean hasil = false;
  
  if(key.length()==64)
  {
    int count =0;
    for (int i=0;i<=63;i++)
    {
      char x = key[i];
      
      if(!isValidCharRSK(x))
        return false;
      else
        count++;
    }
    if(count==64)
      return true;
  }
}

boolean isValidCharRSK(char m)
{
  if((m=='1')||(m=='2')||(m=='3')||(m=='4')||(m=='5')||(m=='6')||(m=='7')||(m=='8')||(m=='9')||(m=='0')||(m=='a')||(m=='b')||(m=='c')||(m=='d')||(m=='e')||(m=='f'))
  {
    return true;
  }
  else
    return false;
}

int valueOfHex(char m)
{
  if(m=='0')
    return 0;
  else if(m=='1')
    return 1;
  else if(m=='2')
    return 2;
  else if(m=='3')
    return 3;
  else if(m=='4')
    return 4;
  else if(m=='5')
    return 5;
  else if(m=='6')
    return 6;
  else if(m=='7')
    return 7;
  else if(m=='8')
    return 8;
  else if(m=='9')
    return 9;
  else if(m=='a')
    return 10;
  else if(m=='b')
    return 11;
  else if(m=='c')
    return 12;
  else if(m=='d')
    return 13;
  else if(m=='e')
    return 14;
  else if(m=='f')
    return 15;
}

void modeLogin() // Mode 4
{
    printToOLED("**Login Mode**",1);
    String unamewallet = readKey(65,77);
    Serial2.print(unamewallet);
    var=0;
    readString="";
    waitfromHPmodified("true","Waiting for Login Information");
}

//------------------------------- START OF SHA FUNCTION -----------------------------------------------------------//
static const uint32_t k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void sha256_transform(SHA256_CTX *ctx, const uint8_t data[]) {
  uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

  for (i = 0, j = 0; i < 16; ++i, j += 4)
    m[i] = ((uint32_t)data[j] << 24) | ((uint32_t)data[j + 1] << 16) | ((uint32_t)data[j + 2] << 8) | ((uint32_t)data[j + 3]);
  for ( ; i < 64; ++i)
    m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];
  f = ctx->state[5];
  g = ctx->state[6];
  h = ctx->state[7];

  for (i = 0; i < 64; ++i) {
    t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
    t2 = EP0(a) + MAJ(a,b,c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0] = 0x6a09e667;
  ctx->state[1] = 0xbb67ae85;
  ctx->state[2] = 0x3c6ef372;
  ctx->state[3] = 0xa54ff53a;
  ctx->state[4] = 0x510e527f;
  ctx->state[5] = 0x9b05688c;
  ctx->state[6] = 0x1f83d9ab;
  ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len) {
  uint32_t i;

  for (i = 0; i < len; ++i) {
    ctx->data[ctx->datalen] = data[i];
    ctx->datalen++;
    if (ctx->datalen == 64) {
      sha256_transform(ctx, ctx->data);
      ctx->bitlen += 512;
      ctx->datalen = 0;
    }
  }
}

void sha256_final(SHA256_CTX *ctx, uint8_t hash[]) {
  uint32_t i;

  i = ctx->datalen;

  // Pad whatever data is left in the buffer.
  if (ctx->datalen < 56) {
    ctx->data[i++] = 0x80;
    while (i < 56)
      ctx->data[i++] = 0x00;
  }
  else {
    ctx->data[i++] = 0x80;
    while (i < 64)
      ctx->data[i++] = 0x00;
    sha256_transform(ctx, ctx->data);
    memset(ctx->data, 0, 56);
  }

  // Append to the padding the total message's length in bits and transform.
  ctx->bitlen += ctx->datalen * 8;
  ctx->data[63] = ctx->bitlen;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  sha256_transform(ctx, ctx->data);

  // Since this implementation uses little endian byte ordering and SHA uses big endian,
  // reverse all the bytes when copying the final state to the output hash.
  for (i = 0; i < 4; ++i) {
    hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
  }
}

char *btoh(char *dest, uint8_t *src, int len) {
  char *d = dest;
  while( len-- ) sprintf(d, "%02x", (unsigned char)*src++), d += 2;
  return dest;
}

String SHA256(String data) 
{
  uint8_t data_buffer[data.length()];
  
  for(int i=0; i<data.length(); i++)
  {
    data_buffer[i] = (uint8_t)data.charAt(i);
  }
  
  SHA256_CTX ctx;
  ctx.datalen = 0;
  ctx.bitlen = 512;
  
  sha256_init(&ctx);
  sha256_update(&ctx, data_buffer, data.length());
  sha256_final(&ctx, hash);
  return(btoh(hex, hash, 32));
}
//------------------------------------- END OF SHA FUNCTION ---------------------------------------------------------//
//----------------------------------- START OF Ed25519 Function-----------------------------------------------------//

void printNumber(const char *name, const uint8_t *x)
{
    static const char hexchars[] = "0123456789ABCDEF";
    Serial.print(name);
    Serial.print(" = ");
    for (uint8_t posn = 0; posn < 32; ++posn) {
        Serial.print(hexchars[(x[posn] >> 4) & 0x0F]);
        Serial.print(hexchars[x[posn] & 0x0F]);
    }
    Serial.println();
}

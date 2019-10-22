
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

// 32-63 PrivateKey, 10-17 PIN, 18-23 TimeStop, 24-29 NextAuth //65-77
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  EEPROM.begin(512);
  
  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  rtc.adjust(DateTime(__DATE__,__TIME__));}
  
  pinMode(34,INPUT_PULLUP); //rusak
 // pinMode(25,INPUT_PULLUP); // toggle
  pinMode(23,INPUT_PULLUP);//push up button
  pinMode(13,INPUT_PULLUP); // enter
  pinMode(12,INPUT); // batlev
  // put your setup code here, to run once:
  //ThisKeyIsReal!!!ThisKeyIsReal!!!
 // delay(5000);
  Serial.println("Back to Setup");
  
   //-----------Kondisi Bongkar tanpa Power---------------
   if (digitalRead(34)==HIGH)
  {
    writeKeyEEPROM("Omae wa Mou Shindeiru!",32,64);
    
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
  NextAuth=stringToInt(readKey(24,29));
  Serial.println("NextAuth = "+String(NextAuth));//

  if (TimeNow>=NextAuth)
  {
      writeKeyEEPROM("Omae wa Mou Shindeiru!!",32,64);
  }
  
 /* while (TimeNow>=NextAuth) //Kasus ini ketika didiamkan walletnya tanpa mau authentikasi. harus masukkan pin dan kunci lagi
  {
    Serial.println("Harus Authentikasi nih ceritanya");
    writeKeyEEPROM("Omae wa Mou Shindeiru!!",32,64);
    printToOLEDmultisize("OOPS!","Terlambat masukin PIN ya?",2,1);
    delay(2000);
    
    printToOLED("Insert PIN NOW\nto recover the wallet!!",1);
    
    String lateMode=Serial2.readString();
    while (lateMode!="5"){
      lateMode=Serial2.readString();
    }
    
    String lateAuth=Serial2.readString();
    while (lateAuth==NULL){
      lateAuth=Serial2.readString();
    }
    
    if (lateAuth==readKey(10,17))
    {
      writeKeyEEPROM(String(TimeStop),24,29);
      NextAuth=stringToInt(readKey(24,29));
      printToOLED("Selamat!!\nSekarang masukkan Key!!",1);
       //ThisKeyIsReal!!!ThisKeyIsReal!!!

      String PintokeyMode = Serial2.readString(); // Pengguna disuruh masuk ke mode PIN
      while (PintokeyMode!="4")
      {
        PintokeyMode = Serial2.readString();
      }
      
      String lateKey=Serial2.readString();
      while(lateKey==NULL)
      {
        lateKey=Serial2.readString();
      }
      writeKeyEEPROM(lateKey,32,64);//Mengembalikan kunci yang sempat dishindeirukan.
      printToOLED("Now you can use the wallet!!\nHooray!!!",1);
      delay(3000);
    }
    else
      printToOLED("PIN salah, coba lagi :D",1);
    delay(1000);
  }*/
  ///////////////////////////////////////////////////////////////////////////////////
attachInterrupt(34,rusak,CHANGE);
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
    TimeNow = RTCnow();
    String NextAuthw = readKey(24,29);
    NextAuth = stringToInt(NextAuthw);
    while(!x && !interruptTrigger && (TimeNow<NextAuth-60))  // normal time
    {
        var=0;
        readString="";
        while (Serial2.available()){ 
              char c= Serial2.read();
              readString += c;}
        
        if (readString.length() > 0)   {  
        var = readString.toInt();}
        printToOLED("Nilai Var: "+String(var),1);
        delay(500);
        // bila shinde masuk mode 1
        boolean shindekah = ShindeiruKa();
        
        if(shindekah) 
        {
          var=1;
        }
        
        //Start of the looping state//
        switch(var)
        {
          case 1: // Shindeiru Toki Mode
            {
              printToOLED("Start Mode 1",1);
              String CekModeShinde = Serial2.readString(); // Pengguna disuruh masuk ke mode PIN
              waitfromHP("219"); //Wait until this value / Connect di shindeiru tokimode
              printToOLED("Sudah connect di Shindeiru Toki Mode",1);
              String pkey= queryfromHP();
              if(pkey!="moemoe") // bila moemoe langsung keluar aja 
                {
                  writeKeyEEPROM(pkey,32,63);
                  var=0;
                  waitfromHP("moemoe");  
                }
              readString="";
              delay(5000);
              break;
            }
          case 2://Misalkan ini yang masukin password
            {
              //Read String from smartphone, then compare to the real PIN. then add the logic for resetState if true.
              //Send Response to PHONE 
              // Dengan ini tidak mungkin masuk ke while bagian bawah
              break;
            }
          case 3: //ModeRegister
            {
              printToOLED("Now Entering Mode Register",1); //Welcome message
              delay(1000);
              String pkNew = randStr(32); // Generate Random
              writeKeyEEPROM(pkNew,32,63); // Write new key to Storage
              writeKeyEEPROM("12345678",10,17); // Default reset PIN when registering
              printToOLED(pkNew+" is written to key space",1);
              delay(1000); 
              Serial2.print(pkNew); //Send pkey to Server
              String newuname = queryfromHP();   // query username dari user
              writeKeyEEPROM(newuname,65,77);
              printToOLED("Username sudah dituliskan :\n"+newuname,1);
              delay(1000); 
              waitformoemoe(); // Cuma nunggu kok
              var=0;
              readString="";
              delay(2000);
              break;
            }
            //
          case 5: //ini untuk input Change PIN
           {
            printToOLED("Now Entering Mode Change PIN",1);
           // Serial2.print("Wallet Siap!");
            
            String pinfromHP= queryfromHP();//pin dr hp
              if(pinfromHP!="moemoe")
                {
                  printToOLED(pinfromHP+" aquired form HP",1);
                  delay(3000);
                  String pinAwal = readKey(10,17);
                    if (pinAwal==pinfromHP) // Bila pin di wallet ama old pin sama,
                    {
                      printToOLED("PIN HP DAN DI DALAM SAMA",1); 
                      Serial2.print("OK");
                      String newPIN  = queryfromHP();
                      if(newPIN!="moemoe") // klo moemoe keluar aj
                      {
                        writeKeyEEPROM(newPIN,10,17);
                        printToOLED(newPIN+" berhasil dicetak ke EEPROM ",1);
                        delay(2000);
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
            
          case 7: //Menu Cek Balance
          {
            printToOLED("Youkoso Cek Balance Mode",1);
            delay(1000);
            String unameMode7 = readKey(65,77);
            printToOLED("Panjang String = "+String(unameMode7.length()),1);
            delay(1000);
            Serial2.print(unameMode7); // Kirim ke HP
            
            
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }

          case 8: //Menu RestoreSK
          {
            printToOLED("Selamat Datang Mode 8",1);
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }
          
          case 9: //Menu Send
          {
            printToOLED("Youkoso Sending Mode",1);
            delay(1000);
            String unameMode9 = readKey(65,77);
            Serial2.print(unameMode9); // Kirim username ke HP
            waitformoemoe(); // wait instruction to exit
            readString="";
            var=0;
            break;
          }
            
          case 4:
            {
              Serial2.print("Masuk");
              printToOLEDmultisize("Insert Key Mode","",1,2);
              String nilaiString;
              boolean insideloop4=true;
              
              while(insideloop4)
              {
                 while(nilaiString==NULL)
                {
                  while(Serial2.available())
                  {
                    nilaiString=Serial2.readString();
                  }
                  
                }
                if (nilaiString=="moemoe")//Keluar
                  {
                    insideloop4=false;
                  }
                  else
                  {
                    writeKeyEEPROM(nilaiString,32,63);
                    printToOLED(nilaiString + " is written to the wallet",1);
                    delay(2000);  
                  }
                    nilaiString=""; 
              }
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
          String privatekey = readKey(32,64);
          Serial.println("Private Key : "+privatekey);
          printToOLEDtriple(detik,tanggal,privatekey,2,1,1);
          delay(1000);
          
          TimeNow=RTCnow();
          TimeStop=RTCnext(TimeNow);
          String uname=readKey(65,77);
          printToOLEDquint(String(TimeNow),String(TimeStop),readKey(24,29),readKey(65,77),readKey(10,17),1,1,1,1,1);
          delay(2500);
          ///--------------------------------------------------------
       // x=digitalRead(25);
        Serial.println("Aku didalam While");
        Serial.println("Waktu sekarang : " + String(TimeNow));
        
        
        
        //// kode di bawah Ini hanya untuk tes, tidak mencerminkan realtime
         /////////////////////////////////////////////////////////////////////


                      
  
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
  display.setTextSize(1);
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
                writeKeyEEPROM(String(TimeStop),24,29);
                printToOLEDmultisize("SUCCEED","New Deadline:\n "+converttoRealTime(TimeStop),1,1);
               delay(3000);
               return true;}
             else
               {printToOLEDmultisize("FAIL","Please Try Again",1,1);
               delay(3000);
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
   printToOLED("menunggu "+desiredValue+" dari HP!" ,1);
   delay(1000);
    while(query!=desiredValue) // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLED("menunggu "+desiredValue+" dari HP!" ,1);
      delay(500);
    }
}

void waitformoemoe() //Wait until phone send moemoe
{
   String query = Serial2.readString(); // disuruh query
   printToOLED("Menunggu Moe",1);
   delay(1000);
    while(query!="moemoe") // Bila tidak sesuai keiinginam, bakal terus didalam
    {
      query=Serial2.readString();
     printToOLED("Menunggu Moe",1);
      delay(500);
    }
}

String queryfromHP() // Menungu respon dari HP
{
    String nilaiString;
    while(nilaiString==NULL)
    {
      while(Serial2.available())
      {
        printToOLED("Nilai query saat ini: "+nilaiString,1);
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

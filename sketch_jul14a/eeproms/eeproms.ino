#include <EEPROM.h>
int resetNotification = 45000;
int resetNow = 60000;
boolean checkpin60 = false;
boolean interruptTrue = false;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
EEPROM.begin(512);
pinMode(34,INPUT_PULLUP);
//pinMode(13,OUTPUT);
//put your main code here, to run repeatedly:
String key = "Arif Sasongko, Moh.Iqbal Arsyad.";
writeKeyEEPROM(key,32,63);
EEPROM.commit();
Serial.println("Key is writed");
Serial.println(char(EEPROM.read(10)));
if (EEPROM.read(10)==255)
 {
  insertInitialPin();
 }
 
}

void loop() {
  attachInterrupt(digitalPinToInterrupt(34),rusak,CHANGE);
  checkpin60=true;
  while ((millis()< resetNotification)&& (checkpin60))
  {
    Serial.print("Time now : ");
    Serial.println(millis());
    String privateKey = readKey(32,63);
    Serial.println(privateKey); 
    String PIN = readKey(10,17);
    Serial.println(PIN);
    Serial.println(char(EEPROM.read(10))); 
    delay(2000);     
  }
  while((millis()<resetNow) &&(checkpin60))
  {
    checkpin60=false;
    String pinValue = readKey(10,17);
    Serial.print("Please rewrite at time= ");
    Serial.print(millis());
    Serial.println(",otherwise it will be overwrited");
    String pass= Serial.readString();
      while((pass==NULL)&& (millis()<=resetNow)&&(!interruptTrue) ) //Entered Password
      { 
      pass= Serial.readString();
      }
    if (pass==pinValue)
    {
      Serial.println("Password Benar");
      checkpin60=true;
    }
    else
    {
      Serial.println("Password Salah");
    }
  }

  if (!checkpin60)
  {
    writeKeyEEPROM("Omae wa mou shindeiru.Shindeiru!",32,64);
    Serial.println("Hore Data anda semua Hilang");
  }
  resetNow+=60000;
  resetNotification+=60000;
  
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

void writeKeyEEPROMnoCommit (String key,int startadd,int untiladd) 
{
  int count = 0;
  for (int i=startadd;i<=untiladd;i++) 
  {
    EEPROM.write(i, key[count]);
    count++;
  }
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

void insertInitialPin()
{
 Serial.println("Masukkan pin pertama anda (8chars):");
 String x=Serial.readString(); 
   while(x==NULL)
   { 
    x= Serial.readString();
    }
 Serial.println("Konfirmasi pin :");
 String x1= Serial.readString(); 
   while(x1==NULL)
   { 
    x1= Serial.readString();
    }
 if (x1==x)
  {
    writeKeyEEPROM(x1,10,17);
    EEPROM.commit();
  }
 else
 {
   Serial.println("Ngetik aj ga bisa. Mau ngibul??");
 }
}


void rusak()
{
   checkpin60=false;
   Serial.println("Shindeiru");
   detachInterrupt(34);
}

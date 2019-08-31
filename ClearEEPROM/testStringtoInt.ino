#include<EEPROM.h>

//Use 120-125
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
EEPROM.begin(512);
writeKeyEEPROM("176184",24,29);
}

void loop() {
  // put your main code here, to run repeatedly:

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

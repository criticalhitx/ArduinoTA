#include <EEPROM.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);

  String key = "Arif Sasongko, Moh.Iqbal Arsyad.";
  Serial.println(key);
  writeKeyEEPROM(key);
  String privateKey = readKey();
  Serial.println(privateKey);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0;i<50;i++)
  {
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(EEPROM.read(i));
    /*if (EEPROM.read(i)==NULL)
     {
        Serial.print("Alamat ke- ");
        Serial.print(i);
        Serial.print(" = TIDAK ADA");
      }*/
  }
  delay(10000);
}

void writeKeyEEPROM (String key) 
{
  int count = 0;
  for (int i=32;i<64;i++) 
  {
    EEPROM.write(i, key[count]);
    count++;
  }
}

String readKey ()
{
  String key;
   for (int i=32;i<64;i++) 
  {
     char karakter = char(EEPROM.read(i));
     key = String (key+karakter);
  }
return key;
}

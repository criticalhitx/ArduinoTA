#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
EEPROM.begin(512);
pinMode(34,INPUT_PULLUP);
pinMode(13,OUTPUT);
/*Serial.println("START WRITING");
EEPROM.write(0, 'A');
EEPROM.write(1, 'B');
EEPROM.write(2, 'C');
EEPROM.write(3, 'D');
EEPROM.commit();
Serial.println("Finished of Writing");
Serial.println("Display : ");*/
//"Arif Sasongko, Moh.Iqbal Arsyad."

String key = "Arif Sasongko, Moh.Iqbal Arsyad.";
Serial.println(key);
writeKeyEEPROM(key);
String privateKey = readKey();
Serial.println(privateKey);

delay(1000);
}

void loop() {
    digitalWrite(13,HIGH);
    }
delay(100);
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

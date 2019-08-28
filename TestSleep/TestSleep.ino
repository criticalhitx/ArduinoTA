#include <esp_sleep.h>
boolean x=true;
boolean resetState=false;
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("Back to Setup");
  pinMode(34,INPUT_PULLUP);
  pinMode(25,INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("LewatSiniKah???"); 
delay(200);
attachInterrupt(34,rusak,CHANGE);
  x=digitalRead(25);
  if (x) //Bila PIN 25 rendah, Masuk mode light sleep
  {
   Serial.println("Mau Tidur zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
     delay(100);
   lightSleep(); 
  }
  else
  {
    Serial.println("Aku Tidak Tertidur");
  }
  
  //While function for active condition [&& pin30==HIGH]
  //Gunakan fungsi IF disini untuk  a)bila TimeNow>=TimeStop ; b) menentukan apakan PIN sama ;||Compare saja 2 alamat di EEPROM, bila tidak sama = shindeiru//
  //If baterai<10 persen, shindeiru
  
Serial.println("Hey Tayo Hey Tayo");
delay(1000);
}

void lightSleep()
{
  esp_sleep_enable_timer_wakeup(10000000); // Timer Sleep
  gpio_wakeup_enable(GPIO_NUM_25,GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(GPIO_NUM_34,GPIO_INTR_HIGH_LEVEL);//GPIO PIN
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  print_wakeup_reason();
}
void rusak()
{
  //writeEEPROMkey("Omae wa mou shindeiru",32,64);
  Serial.println("Shinde");
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

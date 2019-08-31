//GND->SW->9V->(divider resistor || Vin). divider to PIN34
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(34,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
   attachInterrupt(34,rusak,RISING);
  Serial.println("Aishiteru");
  delay(3000);
}

void rusak()
{
  Serial.println("Switch Triggered");
  detachInterrupt(34);
}

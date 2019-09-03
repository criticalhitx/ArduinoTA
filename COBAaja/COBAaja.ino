String x;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
 x="";

}

void loop() {
  // put your main code here, to run repeatedly:
if(x==NULL)
  Serial.println("Kosong");
else
  Serial.println("else");

  delay(1000);
}

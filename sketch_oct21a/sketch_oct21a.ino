

void setup() {
  Serial.begin(115200);
  String x = randStr(32);
  Serial.println("Nilai x" + x);
  Serial.println("Panjang x" + String(x.length()));

}

void loop()
{
  
}

String randStr (int numBytes)
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

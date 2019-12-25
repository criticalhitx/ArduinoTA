#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* #define ED25519_DLL */
#include "src/ed25519.h"
#include "src/ge.h"
#include "src/sc.h"


void setup() {
    Serial.begin(115200);
    GenerateAndPutPrivateKey();
}

void loop()
{
  
}

void GenerateAndPutPrivateKey()
{
    unsigned char public_key[32], private_key[32], seed[32];
    
    /* create a random seed, and a keypair out of that seed */
    ed25519_create_seed(seed);
    ed25519_create_seed(seed);
    ed25519_create_keypair(public_key, private_key, seed);

    String hasil;
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
}

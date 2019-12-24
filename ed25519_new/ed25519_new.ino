#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* #define ED25519_DLL */
#include "src/ed25519.h"
#include "src/ge.h"
#include "src/sc.h"


void setup() {
    Serial.begin(115200);
    unsigned char public_key[32], private_key[64], seed[32], scalar[32];
    unsigned char other_public_key[32], other_private_key[64];
    unsigned char shared_secret[32], other_shared_secret[32];
    unsigned char signature[64];

    clock_t start;
    clock_t end;
    int i;

    const unsigned char message[] = "Hello, world!";
    const int message_len = strlen((char*) message);

    /* create a random seed, and a keypair out of that seed */
    ed25519_create_seed(seed);
    ed25519_create_keypair(public_key, private_key, seed);

    String hasil;
    for (int j=0;j<32;j++)
    {
      String karakter= String(public_key[j]);
      int leng = karakter.length();
      if(leng==1)
      {
        karakter="0"+karakter;
      }
      Serial.println(j+" = "+karakter);
    }
    /* create signature on the message with the keypair */
    
    /*
     ed25519_sign(signature, message, message_len, public_key, private_key);

    // verify the signature
    if (ed25519_verify(signature, message, message_len, public_key)) {
        Serial.println("valid signature\n");
    } else {
        Serial.println("invalid signature\n");
    }

    // create scalar and add it to the keypair
    ed25519_create_seed(scalar);
    ed25519_add_scalar(public_key, private_key, scalar);
    
    // create signature with the new keypair 
    ed25519_sign(signature, message, message_len, public_key, private_key);

    // verify the signature with the new keypair 
    if (ed25519_verify(signature, message, message_len, public_key)) {
        Serial.println("valid signature\n");
    } else {
        Serial.println("invalid signature\n");
    }

    // make a slight adjustment and verify again 
    signature[44] ^= 0x10;
    if (ed25519_verify(signature, message, message_len, public_key)) {
        Serial.println("did not detect signature change\n");
    } else {
        Serial.println("correctly detected signature change\n");
    }

    // generate two keypairs for testing key exchange 
    ed25519_create_seed(seed);
    ed25519_create_keypair(public_key, private_key, seed);
    ed25519_create_seed(seed);
    ed25519_create_keypair(other_public_key, other_private_key, seed);
    
    // create two shared secrets - from both perspectives - and check if they're equal 
    ed25519_key_exchange(shared_secret, other_public_key, private_key);
    ed25519_key_exchange(other_shared_secret, public_key, other_private_key);

    for (i = 0; i < 32; ++i) {
        if (shared_secret[i] != other_shared_secret[i]) {
            Serial.println("key exchange was incorrect\n");
            break;
        }
    }

    if (i == 32) {
        Serial.println("key exchange was correct\n");
    }*/

}
void loop()
{
  
}

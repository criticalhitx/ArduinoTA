/*
 * Copyright (C) 2015 Southern Storm Software, Pty Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
This example runs tests on the Curve25519 algorithm.
*/

#include <Crypto.h>
#include <Curve25519.h>
#include <RNG.h>
#include <string.h>
#include <EEPROM.h>

// Check the eval() function using the test vectors from
// section 6.1 of RFC 7748.
String getPublicKey()
{
    static uint8_t kunci_private[32] = {
        0x77, 0x07, 0x6d, 0x0a, 0x73, 0x18, 0xa5, 0x7d,
        0x3c, 0x16, 0xc1, 0x72, 0x51, 0xb2, 0x66, 0x45,
        0xdf, 0x4c, 0x2f, 0x87, 0xeb, 0xc0, 0x99, 0x2a,
        0xb1, 0x77, 0xfb, 0xa5, 0x1d, 0xb9, 0x2c, 0x2a
    };

    // Fix up the private keys by applying the standard masks.
   // kunci_private[0] &= 0xF8;
   // kunci_private[31] = (kunci_private[31] & 0x7F) | 0x40;

    // Write private key to EEPROM-------------//
    for(int i=32;i<=63;i++)
    {
      EEPROM.write(i,kunci_private[i-32]);
      EEPROM.commit();
    }
    //-----------------------------------------//
    
    // Read private key from EEPROM -----------//
    String myPKeySTR = readPKey();
    Serial.println("My PKEY = "+myPKeySTR);
    //------------------------------------------//
    
    // Evaluate the curve function and check the public keys.
    uint8_t result[32];
    Serial.println("Fixed test vectors:");
    Serial.print("Computing Public key ... ");
    Serial.flush();
    unsigned long start = micros();
    Curve25519::eval(result, kunci_private, 0);
    
    String hasil;
    for (int i=0;i<32;i++)
    {
      String karakter= String(result[i],HEX);
      int leng = karakter.length();
      if(leng==1)
      {
        karakter="0"+karakter;
      }
      hasil+=karakter;
    }
    Serial.println("Hasil Public Key adalah = "+hasil);
    return hasil;
}

/*
void testDH()
{
    static uint8_t alice_k[32];
    static uint8_t alice_f[32];
    static uint8_t bob_k[32];
    static uint8_t bob_f[32];

    Serial.println("Diffie-Hellman key exchange:");
    Serial.print("Generate random k/f for Alice ... ");
    Serial.flush();
    unsigned long start = micros();
    Curve25519::dh1(alice_k, alice_f);
    unsigned long elapsed = micros() - start;
    Serial.print("elapsed ");
    Serial.print(elapsed);
    Serial.println(" us");

    Serial.print("Generate random k/f for Bob ... ");
    Serial.flush();
    start = micros();
    Curve25519::dh1(bob_k, bob_f);
    elapsed = micros() - start;
    Serial.print("elapsed ");
    Serial.print(elapsed);
    Serial.println(" us");

    Serial.print("Generate shared secret for Alice ... ");
    Serial.flush();
    start = micros();
    Curve25519::dh2(bob_k, alice_f);
    elapsed = micros() - start;
    Serial.print("elapsed ");
    Serial.print(elapsed);
    Serial.println(" us");

    Serial.print("Generate shared secret for Bob ... ");
    Serial.flush();
    start = micros();
    Curve25519::dh2(alice_k, bob_f);
    elapsed = micros() - start;
    Serial.print("elapsed ");
    Serial.print(elapsed);
    Serial.println(" us");

    Serial.print("Check that the shared secrets match ... ");
    if (memcmp(alice_k, bob_k, 32) == 0)
        Serial.println("ok");
    else
        Serial.println("failed");
}*/

String readKey (int startadd, int untiladd)
{
  String key;
   for (int i=startadd;i<=untiladd;i++) 
  {
     char karakter = char(EEPROM.read(i));
     if(karakter!=NULL)
        key = String (key+karakter);
     else 
       return key;
     
  }
return key;
}

String readPKey()
{
    String key;
    for(int i=32;i<=63;i++)
    {
       String karakter = String(EEPROM.read(i),HEX);
       if (karakter.length()==1)
       {
        karakter="0"+karakter;
       }
       if(karakter!=NULL)
          key = String (key+karakter);
       else 
         return key;
    }
}

void setup()
{
    Serial.begin(115200);
    EEPROM.begin(512);
    RNG.begin("TestCurve25519 1.0");
    
    //---Receive Mode--------------------
    String myPublicKey=getPublicKey();
    Serial.println(myPublicKey);
    //----------------------------------
    
}

void loop()
{
}

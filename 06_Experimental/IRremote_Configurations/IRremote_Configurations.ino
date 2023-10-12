
#include <IRremote.h>

IRrecv irrecv(6);

decode_results results;

#define BUTON1 0xFFA25D
#define BUTON2 0xFF629D
#define BUTON3 0xFFE21D
#define BUTON4 0xFF22DD
#define BUTON5 0xFF02FD
#define BUTON6 0xFFC23D
#define BUTON7 0xFFE01F
#define BUTON8 0xFFA857
#define BUTON9 0xFF906F
#define YILDIZ 0xFF6897
#define BUTON0 0xFF9867
#define YUKARI 0xFF18E7
#define SOL    0xFF10EF
#define OK     0xFF38C7
#define SAG    0xFF5AA5
#define ASAGI  0xFF4AB5


void setup() 
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() 
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
  
}

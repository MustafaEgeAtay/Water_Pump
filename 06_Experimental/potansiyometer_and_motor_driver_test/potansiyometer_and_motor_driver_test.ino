#include <math.h>

// Deneme amacı ile yapılmıştır.
// ADC hattından gelen veriye göre motor hızı ayarlamaktadır.
// Bu projede ADC'den gelen veri ile ultrasonik sesnsörden gelen
// verinin karşılaştırılması sonucu çıkan hatanın kontrol denklemine
// sokularak PWM hattından orantılı değerler verilmesi ile sağlanacaktır.

#define POTPIN A0
#define PWM_PIN 9
#define IN1 7
#define IN2 6

int deger = 0;

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(PWM_PIN,OUTPUT);
  // IN1 ve IN2 yön ayarlama pinleridir.
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
}
void loop() {
  deger = analogRead(POTPIN); // ADC hattını oku
  // Gelen değeri işle. Motor sürücüye alakalı PWM değerini bas.
  analogWrite(PWM_PIN,floor(deger / 4)); 
}

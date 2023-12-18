#include <math.h>

// Deneme amacı ile yapılmıştır.
// ADC hattından gelen veriye göre motor hızı ayarlamaktadır.
// Bu projede ADC'den gelen veri ile ultrasonik sesnsörden gelen
// verinin karşılaştırılması sonucu çıkan hatanın kontrol denklemine
// sokularak PWM hattından orantılı değerler verilmesi ile sağlanacaktır.


#define PWM_PIN 5
#define IN1 6
#define IN2 7

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
  deger = 180; // ADC hattını oku
  // Gelen değeri işle. Motor sürücüye alakalı PWM değerini bas.
  analogWrite(PWM_PIN,deger); 
}

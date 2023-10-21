//ultraonic sensör ve lcd ekran ayarlari


#include <stdio.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int buttonPin = 7;
int buttonState = 0;
unsigned long ultrasonic_o;
int height=30 ;



NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
    
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  lcd.begin();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
}

void loop() {

  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Yükseklik: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  delay(50);
  ultrasonic_o = sonar.ping_cm();
  Serial.println(ultrasonic_o);

  buttonState = digitalRead(buttonPin);



  

    if (buttonState == HIGH){

  lcd.setCursor(0,0);
  lcd.print("Yukseklik:");
  lcd.setCursor(3,1);
  lcd.print("cm");
  lcd.setCursor(0, 1); // Set cursor to the first column of the second row
  lcd.print("  "); // Clear the second row by printing spaces
  lcd.setCursor(0, 1); // Set cursor to the first column of the second row
  delay(50);
  lcd.print(ultrasonic_o); // Print the new number on the second row
  delay(500);



  } else {

  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("istenen:");
  lcd.setCursor(0,1);
  lcd.print(height);
  
  }



}

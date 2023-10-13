/ultraonic sensör ve lcd ekran ayarlari


#include <stdio.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int ultrasonic_o;
int yükeklik_i;





NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
    
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Yükseklik:");
  lcd.setCursor(0,1);
  lcd.print(yükseklik_i);

}

void loop() {
  
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Yükseklik: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");:

}












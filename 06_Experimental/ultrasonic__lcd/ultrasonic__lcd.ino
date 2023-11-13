//ultraonic sensör ve lcd ekran ayarlari

#include <stdio.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define POTPIN A0

#define LEVER_SWITCH_PIN1 2
#define LEVER_SWITCH_PIN2 3
#define LEVER_SWITCH_PIN3 4

int pressSwitch[3] ;
int mode;
int previousMode;


int buttonState = 0;
unsigned long ultrasonic_o;
int deger;
int previousDeger = 0;
unsigned long changeTime = 0; // Variable to store the time of the last change
unsigned long waitTime = 3000;

enum menu_state{
    current_distance = 0,
    wanted_distance = 1,
    current_mode = 2
}menu_current_state;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
    
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  lcd.begin();
  lcd.backlight();

  pinMode(LEVER_SWITCH_PIN1,INPUT);
  pinMode(LEVER_SWITCH_PIN2,INPUT);
  pinMode(LEVER_SWITCH_PIN3,INPUT);

  menu_current_state = current_distance;
  
}

void loop() {

  // -------------- Read Variables --------------

  deger = analogRead(POTPIN)/56;
  
  ultrasonic_o = sonar.ping_cm();
 

  pressSwitch[2] = digitalRead(LEVER_SWITCH_PIN1);
  pressSwitch[1] = digitalRead(LEVER_SWITCH_PIN2);
  pressSwitch[0] = digitalRead(LEVER_SWITCH_PIN3);
  
  mode = 4*pressSwitch[2] + 2*pressSwitch[1] + pressSwitch[0];

  // -------------- Read Variables --------------

  // -------------- LCD Screen Output --------------

  if((deger != previousDeger) || (menu_current_state == wanted_distance)){
    // The target distance (istenen) has changed
   
    lcd.setCursor(0, 0);
    lcd.print("istenen:   ");
    lcd.setCursor(3, 1);
    lcd.print("cm");
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    delay(50);
    lcd.print(deger);
    delay(50);
    
    // Record the time of the change
    if(deger != previousDeger ){
        changeTime = millis();
        menu_current_state = wanted_distance;
        previousDeger = deger;
    }
    
    if(millis() - changeTime >= waitTime){
        menu_current_state = current_distance;
    }
  
  }

  if ((mode != previousMode) || (menu_current_state == current_mode)) {
    // The mode has changed
    lcd.setCursor(0, 0);
    lcd.print("Mod:      ");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    delay(50);
    lcd.setCursor(0, 1);
    lcd.print(mode);
    delay(50);
  
    // Record the time of the change
    if(mode != previousMode){
        changeTime = millis();
        menu_current_state = current_mode;
        previousMode = mode;
    }

    if(millis() - changeTime >= waitTime){
        menu_current_state = current_distance;
    }
  
  }
  
  if(menu_current_state == current_distance){
    
    // Wait for 3 seconds before exiting the if block
    lcd.setCursor(0, 0);
    lcd.print("Yukseklik:");
    lcd.setCursor(3, 1);
    lcd.print("cm");
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    
    
    lcd.print(ultrasonic_o);
    delay(100);

    
  }

  // -------------- LCD Screen Output --------------
  
    Serial.println(mode);
    
}


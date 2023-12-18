//ultraonic sensör ve lcd ekran ayarlari

#include <stdio.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <math.h>


#define PWM_PIN 5
#define IN1 6
#define IN2 7

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
int water_level;
int deger;
int previousDeger = 0;
unsigned long changeTime = 0; // Variable to store the time of the last change
unsigned long waitTime = 3000;

int Error = 0;
int Integral_Error = 0;
int Prev_Error=0;
int Derivative=0;


enum menu_state{
    current_distance = 0,
    wanted_distance = 1,
    current_mode = 2
}menu_current_state;


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

typedef struct{
    float Kp=30;
    double Value_Error;
}P_Object_Typedef;

typedef struct{
    float Kp=15;
    float Ki=0.3;
    double Value_Error;
}PI_Object_Typedef;

typedef struct{
    float Kp=27;
    float Kd=2;
    double Value_Error;
}PD_Object_Typedef;

typedef struct{
    float Kp=25;
    float Kd=0.5;
    float Ki=0.3;
    double Value_Error;
}PID_Object_Typedef;

P_Object_Typedef P_Object;
PI_Object_Typedef PI_Object;
PD_Object_Typedef PD_Object;
PID_Object_Typedef PID_Object;




void setup() {
    
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  lcd.begin();
  lcd.backlight();

  pinMode(LEVER_SWITCH_PIN1,INPUT);
  pinMode(LEVER_SWITCH_PIN2,INPUT);
  pinMode(LEVER_SWITCH_PIN3,INPUT);
  menu_current_state = current_distance;

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(PWM_PIN,OUTPUT);
  
}

void loop() {

  // -------------- Read Variables --------------

  deger = analogRead(POTPIN)/84;
  
  ultrasonic_o = sonar.ping_cm();

  water_level = 12 - ultrasonic_o;
  water_level = fabs(water_level);


  Error = deger-water_level;
  Integral_Error+= Error; 
  Derivative = Error-Prev_Error;
 
  

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
        Integral_Error= 0; 
        Derivative = 0;
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
    
    
    lcd.print(water_level);
    delay(100);

    
  }

  // -------------- LCD Screen Output --------------
  
    Serial.println(mode);

  //-------------------Controls


  switch(mode){

    case 0:
      //on-off

      if (Error>0){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        analogWrite(PWM_PIN,200);
          

      }else if (Error<0) {

        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        analogWrite(PWM_PIN,200); 

      }else if (Error ==0){

        analogWrite(PWM_PIN,0); 

      }
    
    break;

    case 1:
      //p


      P_Object.Value_Error =  P_Object.Kp*Error;

      if (Error>0){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);


        if (P_Object.Value_Error>200){

          P_Object.Value_Error=200;

        }else if (P_Object.Value_Error<90){

          P_Object.Value_Error=0;

        }
        

      
        analogWrite(PWM_PIN,P_Object.Value_Error);
          

      }else if (Error<0){

        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        

          if (P_Object.Value_Error<-200){

          P_Object.Value_Error=-200;

        }else if (P_Object.Value_Error>-90){

          P_Object.Value_Error=0;

        } 

        analogWrite(PWM_PIN,fabs(P_Object.Value_Error));

      }else if(Error==0){

        analogWrite(PWM_PIN,0);
        
      }
      
    break;

    case 2:

      

      PI_Object.Value_Error =  PI_Object.Kp*Error + PI_Object.Ki*Integral_Error;

      if (Error>0){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);


      if (PI_Object.Value_Error>200){

        PI_Object.Value_Error=200;

      }else if (PI_Object.Value_Error<90){

        PI_Object.Value_Error=0;

      }

       analogWrite(PWM_PIN,PI_Object.Value_Error);
          

      }else if (Error<0){

        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        

          if (PI_Object.Value_Error<-200){

          PI_Object.Value_Error=-200;

        }else if (PI_Object.Value_Error>-90){

          PI_Object.Value_Error=0;

        } 

        analogWrite(PWM_PIN,fabs(PI_Object.Value_Error));

      }else if(Error==0){

        analogWrite(PWM_PIN,0);
        
      }

    break;

    case 3:

      PD_Object.Value_Error =  PD_Object.Kp*Error + PD_Object.Kd*Derivative;
      Prev_Error = Error;

      if (Error>0){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);


      if (PD_Object.Value_Error>200){

        PD_Object.Value_Error=200;

      }else if (PD_Object.Value_Error<90){

        PD_Object.Value_Error=0;

      }

       analogWrite(PWM_PIN,PD_Object.Value_Error);
          

      }else if (Error<0){

        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        

          if (PD_Object.Value_Error<-200){

          PD_Object.Value_Error=-200;

        }else if (PD_Object.Value_Error>-90){

          PD_Object.Value_Error=0;

        } 

        analogWrite(PWM_PIN,fabs(PD_Object.Value_Error));

      }else if(Error==0){

        analogWrite(PWM_PIN,0);
        
      }
    
    break;

    case 4:

      PID_Object.Value_Error =  PID_Object.Kp*Error + PID_Object.Kd*Derivative + PID_Object.Ki*Integral_Error;
      Prev_Error = Error;

      if (Error>0){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);


      if (PID_Object.Value_Error>200){

        PID_Object.Value_Error=200;

      }else if (PID_Object.Value_Error<90){

        PID_Object.Value_Error=0;

      }

       analogWrite(PWM_PIN,PID_Object.Value_Error);
          

      }else if (Error<0){

        digitalWrite(IN1,LOW);
        digitalWrite(IN2,HIGH);
        

          if (PID_Object.Value_Error<-200){

          PID_Object.Value_Error=-200;

        }else if (PID_Object.Value_Error>-90){

          PID_Object.Value_Error=0;

        } 

        analogWrite(PWM_PIN,fabs(PID_Object.Value_Error));

      }else if(Error==0){

        analogWrite(PWM_PIN,0);
        
      }      

    default:

    break;

  }

}










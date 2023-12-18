/*******************************************************************************
 * @File        : Water_Pump.c
 * @Created_on  : 13 Kas 2023
 * @Author      : Mustafa Ege Atay, Arda Dogan
 * @Email       : , dgnarda@gmail.com
 * @About       : 
 * @Company     :
 * @Copyright   :
 ******************************************************************************/
/***************************** Defining Header ********************************/
#include "Water_Pump.h"
#include <stdio.h>
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/****************************** Defining Flags ********************************/
Menu_State Menu_Current_State = Current_Distance;

/************************ Defining Private Variables **************************/
int ADC_Input;
int Previous_ADC_Input = 0;

int Switch_Input[3] ;

int Control_Mode;
int Previous_Control_Mode;

unsigned long Change_Time = 0; // Variable to store the time of the last change

unsigned long ultrasonic_o;

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);  


/*******************************************************************************
 * @Function_Name   : Water_Pump_Init
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Setup function for the library.
 ******************************************************************************/
void Water_Pump_Init(void){
  // Open serial monitor at 115200 baud to see ping results.
  Serial.begin(SERIAL_MONITOR_BAUDRATE);
  lcd.begin();
  lcd.backlight();

  pinMode(LEVER_SWITCH_PIN1,INPUT);
  pinMode(LEVER_SWITCH_PIN2,INPUT);
  pinMode(LEVER_SWITCH_PIN3,INPUT);  
  Serial.println("Water_Pumo_Init works");

}

/*******************************************************************************
 * @Function_Name   : Read_Inputs
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Read potentiometer, switch and ultrasonic sensor values.
 ******************************************************************************/
void Water_Pump_Read_Inputs(void){
  ADC_Input = analogRead(POTPIN)/56;
  
  ultrasonic_o = sonar.ping_cm();
  delay(50);

  Switch_Input[2] = digitalRead(LEVER_SWITCH_PIN1);
  Switch_Input[1] = digitalRead(LEVER_SWITCH_PIN2);
  Switch_Input[0] = digitalRead(LEVER_SWITCH_PIN3);
  Control_Mode = 4*Switch_Input[2] + 2*Switch_Input[1] + Switch_Input[0]; 
  Serial.println("Water_Pump_Read_Inputs works"); 
}

/*******************************************************************************
 * @Function_Name   : Water_Pump_LCD_Screen_FSM
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Simple FSM for displaying GUI to LCD screen
 ******************************************************************************/
void Water_Pump_LCD_Screen_FSM(void){
  if((ADC_Input != Previous_ADC_Input) || (Menu_Current_State == Wanted_Distance)){
    // The target distance (istenen) has changed
    lcd.setCursor(0, 0);
    lcd.print("istenen:   ");
    lcd.setCursor(3, 1);
    lcd.print("cm");
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    delay(50);
    lcd.print(ADC_Input);
    
    // Record the time of the change
    if(ADC_Input != Previous_ADC_Input ){
        Change_Time = millis();
        Menu_Current_State = Wanted_Distance;
        Previous_ADC_Input = ADC_Input;
    }
    
    if(millis() - Change_Time >= LCD_DISPLAY_TIME){
        Menu_Current_State = Current_Distance;
    }
  }

  if ((Control_Mode != Previous_Control_Mode) || (Menu_Current_State == Current_Mode)) {
    // The mode has changed
    lcd.setCursor(0, 0);
    lcd.print("Mod:      ");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    delay(50);
    lcd.setCursor(0, 1);
    lcd.print(Control_Mode);
  
    // Record the time of the change
    if(Control_Mode != Previous_Control_Mode){
        Change_Time = millis();
        Menu_Current_State = Current_Mode;
        Previous_Control_Mode = Control_Mode;
    }

    if(millis() - Change_Time >= LCD_DISPLAY_TIME){
        Menu_Current_State = Current_Distance;
    }
  }
  
  if(Menu_Current_State == Current_Distance){ 
    // Wait for 3 seconds before exiting the if block
    lcd.setCursor(0, 0);
    lcd.print("Yukseklik:");
    lcd.setCursor(3, 1);
    lcd.print("cm");
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    
    // delay(500);
    lcd.print(ultrasonic_o);
  }

  Serial.println("Water_Pump_LCD_Screen_FSM works"); 
  
}

 

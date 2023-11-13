/*******************************************************************************
 * @File        : Water_Pump.h
 * @Created_on  : 13 Kas 2023
 * @Author      : Mustafa Ege Atay, Arda Dogan
 * @Email       : , dgnarda@gmail.com
 * @About       : 
 * @Company     : 
 * @Copyright   :
 ******************************************************************************/

#ifndef WATER_PUMP_H_
#define WATER_PUMP_H_

/***************************** Private Includes *******************************/

/***************************** Private Externs ********************************/

/**************************** Defining Constants ******************************/

#define SERIAL_MONITOR_BAUDRATE   115200
#define LCD_DISPLAY_TIME          3000

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define POTPIN A0

#define LEVER_SWITCH_PIN1 2
#define LEVER_SWITCH_PIN2 3
#define LEVER_SWITCH_PIN3 4

/******************************* Defining Enums *******************************/
typedef enum {
    Current_Distance = 0,
    Wanted_Distance = 1,
    Current_Mode = 2
}Menu_State;

/************************* Defining Function Protypes *************************/
void Water_Pump_Init(void);
void Water_Pump_Read_Inputs(void);
void Water_Pump_LCD_Screen_FSM(void);

#endif
/***************************** End of File ************************************/

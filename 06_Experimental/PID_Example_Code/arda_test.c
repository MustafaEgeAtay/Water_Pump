#include <stdlib.h>

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    double Current_Value;
    double Value_Error;
}PID_Object_Typedef;

int Ultra_Sonic_Value = 0;
int Reference = 1;

PID_Object_Typedef PID_Object;


int Error = 0;
int Prev_Error = 0;
int Past_Error = 0;


void  PID_Calculation(PID_Object_Typedef Object){
    Object.Value_Error +=  Object.Kp * Error;
    Object.Value_Error +=  Object.Ki * Past_Error;
    Object.Value_Error +=  Object.Kd * (Error - Prev_Error);
}


int main(){

    // Ultra sonik ve reference güncelleniyor

    Past_Error += Error; 
    Prev_Error = Error;
    Error = Reference - Ultra_Sonic_Value;

    // Switchlerden kontrol mod tipi input olarak al.

    switch(input_mode){

        case On_Off:

            break;

        case PID_Control:
            PID_Calculation(PID_Object);
            break;

        default:

            break;

    }
    
    return 0;
}
/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project_utils.h"

double pos_servo1_rad;
double pos_servo2_rad;

uint16_t pos_servo1;
uint16_t pos_servo2;

double val = PI / SERVO_LIMIT_H;

/*float x_array[COORD_ARRAY_LENGTH];
float y_array[COORD_ARRAY_LENGTH];
float z_array[COORD_ARRAY_LENGTH];*/

extern uint8_t received;


void start_components ()
{
    //Init components
    Timer_HCSR04_Start();
    PWM_Trigger_Start();
    Timer_TRIGGER_Start();
    UART_1_Start();
    isr_1_StartEx(Custom_UART_RX_ISR);
    PWM_Servo1_Start();
    PWM_Servo2_Start();
    // Call the Custom_ISR_Start function -- defined in isr.c
    Custom_ISR_Start();
    /* Send message over UART */
    UART_1_PutString("HC-SR04 Program Started\r\n");
}

void set_servos (uint8_t servo_1, uint8_t servo_2)
{
    //this function sets the position of the servo motors to the two limit positions
    //write 0 in the dedicated variable to set position to 0 degrees
    //write 1 in the corresponding variable to set position to 180 degrees

    if (!servo_1)
        Servo_SetPosition1(SERVO_LIMIT_L);
    else if (servo_1)
        Servo_SetPosition1(SERVO_LIMIT_H);
    if (!servo_2)
        Servo_SetPosition2(SERVO_LIMIT_L);
    else if (servo_2)
        Servo_SetPosition2(SERVO_LIMIT_H);
}



void find_position ()
{
    //this function compute the coordinates of a three dimensional point 
    //thanks to the position returned by the two servos and the distance returned 
    //by the USS
    
    pos_servo1 = Servo_GetPosition1(); 
    pos_servo2 = Servo_GetPosition2(); 


    //CONSIDERO L'ANGOLO IN RADIANTI PER FUNZIONE SIN E COS
    pos_servo1_rad=(pos_servo1-SERVO_MID_ANGLE)*val;
    pos_servo2_rad=(pos_servo2-SERVO_MID_ANGLE)*val;
    
    
    //CALCOLO COORDINATE PUNTO INDIVIDUATO
    
    x=D2*(sin(pos_servo1_rad))+(ARM_LENGTH+distance)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    y=D2*cos(pos_servo1_rad)+(distance+ARM_LENGTH)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    z=Z1+(distance+ARM_LENGTH)*sin(pos_servo2_rad);
}

void next_row (void)
{
    angle_2=angle_2+STEP_RISE;
    Servo_SetPosition2(angle_2);
    if (angle_2==RISE_LIMIT)
    {
        angle_2=SERVO_LIMIT_L; 
        state = DISPLAY;
    }
}

void sweep (uint8_t start_position, uint8_t end_position, uint8_t direction)
{
    if (direction == SX)
    {
        angle= start_position;
        do
        {
           Servo_SetPosition1(angle*step_sweep); 
            CyDelay(SWEEP_DELAY);
            
            find_position();
            //capire se find position non deve restituire x y z
            sprintf (message, "%d %d %d\r\n", (int)x,(int)y,(int)z);

            UART_1_PutString(message);  
            if (angle == end_position)
            {   
                next_row();
                CyDelay(RISE_DELAY); 
                state = SCAN_DX;
            }
            angle ++;
        } while (angle<=end_position && state != IDLE);      
    }
    if (direction == DX)
    {
        angle=start_position;
        do
        {
            Servo_SetPosition1(angle*step_sweep);
            CyDelay(SWEEP_DELAY);
            find_position();
            //capire se find position non deve restituire x y z
            sprintf (message, "%d %d %d\r\n", (int)x,(int)y,(int)z);

            UART_1_PutString(message);
            if (angle == (end_position+1))
            {   
                next_row();
                CyDelay(RISE_DELAY); 
                state = SCAN_SX;
            }
            angle --;
        } while (angle >end_position && state != IDLE);
    }
}


/* [] END OF FILE */

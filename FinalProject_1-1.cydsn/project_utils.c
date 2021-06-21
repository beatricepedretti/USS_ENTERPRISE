/* =================================================
 *
 * U.S.S. E.N.T.E.R.P.R.I.S.E.
 * Ultrasound Sensor for Environment Recording 
 * and Reconstruction Integrated Scanning Equipment
 * 
 * Electronic Technologies and Biosensors Laboratory
 * Final Project
 * 
 * Di Liddo, Goshen, Pedretti
 *
 * =================================================
*/

#include "project_utils.h"

double pos_servo1_rad;
double pos_servo2_rad;

uint16_t pos_servo1;
uint16_t pos_servo2;

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
    //UART_1_PutString("HC-SR04 Program Started\r\n");
}

void find_position ()
{
    //this function compute the coordinates of a three dimensional point 
    //thanks to the position returned by the two servos and the distance returned 
    //by the USS
    
    pos_servo1 = Servo_GetPosition1(); 
    pos_servo2 = Servo_GetPosition2(); 

    //conversion from degrees to rad
    pos_servo1_rad=(SERVO_MID_ANGLE-pos_servo1)*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(SERVO_MID_ANGLE-pos_servo2)*(PI / SERVO_LIMIT_H);
    
    
    //pos_servo1_rad=(pos_servo1-SERVO_MID_ANGLE)*(PI / SERVO_LIMIT_H);
    //pos_servo2_rad=(pos_servo2-SERVO_MID_ANGLE)*(PI / SERVO_LIMIT_H);
    
    //computation of coordinates    
    x=D2*(sin(pos_servo1_rad))+(ARM_LENGTH+distance)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    y=D2*cos(pos_servo1_rad)+(distance+ARM_LENGTH)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    z=Z1+(distance+ARM_LENGTH)*sin(pos_servo2_rad);
    sprintf (message, "%d %d %d\r\n", (int)x,(int)y,(int)z);
    UART_1_PutString(message);
}

void next_row (void)
{
    //this function increase the angle of the second servo motor allowing the scanning of the next "layer" of the environment
    angle_2=angle_2+STEP_RISE;
    Servo_SetPosition2(angle_2);
    CyDelay(RISE_DELAY);
    if (angle_2==RISE_LIMIT)
    {
        angle_2=SERVO_LIMIT_L; 
        state = DISPLAY;
    }
}

void sweep (uint8_t start_position, uint8_t end_position, uint8_t direction)
{
    //this function takes as inputs the start  and end position and the direction.
    //it performs the scanning of the angle that exists between the start and end position
    //with step: step_sweep
    if (Servo_GetPosition1() != start_position)
        angle= start_position; 
   
    
    if (direction == SX)
    {      
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

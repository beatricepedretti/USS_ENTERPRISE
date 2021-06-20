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

uint8_t d_2=13; //di quanto l'aggancio del servo2 è spostata rispetto all'albero del servo1
uint8_t Z1 = 73; //altezza albero servo2 in mm, quindi altezza servo1 + metà spessore servo2. Ho messo 30 a caso
uint8_t aggancio_sonar = 57;//da misurare


/*float x_array[COORD_ARRAY_LENGTH];
float y_array[COORD_ARRAY_LENGTH];
float z_array[COORD_ARRAY_LENGTH];*/

extern char received;
extern int begin_scann;


extern volatile uint8_t clockwise;


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
    
    /*
    sprintf(message_1, "coord: %d mm\r\n", (int)pos_servo1_rad);
    UART_1_PutString(message_1);
    */
    
    //CALCOLO COORDINATE PUNTO INDIVIDUATO
    
    X=d_2*(sin(pos_servo1_rad))+(aggancio_sonar+distance)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    Y=d_2*cos(pos_servo1_rad)+(distance+aggancio_sonar)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    Z=Z1+(distance+aggancio_sonar)*sin(pos_servo2_rad);
    
    //X = sin(pos_servo1_rad)*(d_2+(distance+aggancio_sonar)); //-90 se consideriamo lo zero di pos_servo1 l'angolo totalmente a sinistra (180 gradi)
    //Z = Z1+(distance+aggancio_sonar)*sin(pos_servo2_rad); //-90 sempre se consideriamo lo zero l'angolo totalmente a sinistra (quello che farà scendere il sonar di altezza)
    //Y = (d_2+distance+aggancio_sonar)*cos(pos_servo1_rad);
    
   
    /*
    sprintf(message_1, "coord: %d mm\r\n", (int)Z);
    UART_1_PutString(message_1);
    */
      
}

void next_row (void)
{
    angle_2=angle_2+STEP_RISE;
    Servo_SetPosition2(angle_2);
    if (angle_2==RISE_LIMIT)
        angle_2=SERVO_LIMIT_L;     
}


void check_state ()
{
    if(received=='s'){
        begin_scann=0;
        //STEP_SWEEP=1;
        
    }
}


/*
void sweep (uint8_t low_position, uint8_t high_position)
{
    if (clockwise == 0)
    {           
        for (angle=low_position; angle<=(high_position/STEP_SWEEP); angle ++)
        {
            Servo_SetPosition1(angle*STEP_SWEEP);
            if (angle == (high_position/STEP_SWEEP))
            {                    
                clockwise=1;
                CyDelay(SWEEP_DELAY);
                next_row();
            }
            CyDelay(RISE_DELAY);     
        }
    }
    if (clockwise == 1)
    {
        for (angle=(high_position/STEP_SWEEP);angle >low_position; angle --)
        {
            Servo_SetPosition1(angle*STEP_SWEEP);
            if (angle == (low_position+1))
            {
                clockwise = 0;
                Servo_SetPosition1(low_position);
                CyDelay(SWEEP_DELAY);
                next_row();
                
            }
            
            CyDelay(RISE_DELAY);
        }
    }
}
*/

/* [] END OF FILE */

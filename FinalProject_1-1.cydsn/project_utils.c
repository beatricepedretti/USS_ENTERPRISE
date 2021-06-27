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

void start_components ()
{
    //Init components
    Timer_HCSR04_Start();
    PWM_Trigger_Start();
    Timer_TRIGGER_Start();
    UART_Start();
    isr_UART_StartEx(Custom_UART_RX_ISR);
    PWM_Servo1_Start();
    PWM_Servo2_Start();
    // Call the Custom_ISR_Start function -- defined in isr.c
    Custom_ISR_Start();
}

void next_row (void)
{
    //this function increases the angle of the second servo motor allowing the scanning of the next "layer" of the environment
    angle_2=angle_2+STEP_RISE;
    Servo_SetPosition2(angle_2);
    CyDelay(RISE_DELAY);
    //if I have ended the scanning, go to DISPLAY state
    if (angle_2==RISE_LIMIT)
    {
        angle_2=SERVO_LIMIT_L; 
        state = DISPLAY;
    }
}

//this function resets all variables except for step_sweep which can be changed in the GUI
void reset_variables ()
{
    start_position = SCAN_LIMIT_L;
    end_position =  SCAN_LIMIT_H;
    direction = LEFT;
    angle_2 = SERVO_LIMIT_L;
    flag_print = 0;
    flag_connected  = 0;
}


void stop_components ()
{
    //Init components
    Timer_HCSR04_Stop();
    PWM_Trigger_Stop();
    Timer_TRIGGER_Stop();
    PWM_Servo1_Stop();
    PWM_Servo2_Stop();
    ISR_HCSR04_Disable();
}


/* [] END OF FILE */

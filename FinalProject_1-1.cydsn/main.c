/* =========================================================
 *
 * U.S.S. E.N.T.E.R.P.R.I.S.E.
 * Ultrasound Sensor for ENvironmenT Recording and
 * Point-cloud Reconstruction Integrated Scanning Equipment
 * 
 * Electronic Technologies and Biosensors Laboratory
 * Final Project
 * 
 * Di Liddo, Goshen, Pedretti
 *
 * =========================================================
*/

#include "project_utils.h"

int main(void)
{ 
    uint16_t angle;
    uint8_t temp_position;
    
    state = IDLE;
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position if they are not already 
    if (Servo_GetPosition1()!=SCAN_LIMIT_L || Servo_GetPosition2 ()!= SERVO_LIMIT_L)
        reset_servos();
    //every time I turn on the device, reset variables
    reset_variables();
    //reset sweep step to 5 degrees in case the step is not selected by the user
    step_sweep = 5;
    
        
    for(;;)
    {
        switch (state)
        {   
            //IDLE state: wait for user to press "START" in GUI
            //while in idle state, the user can select the step for the sweep between two choices
            case IDLE:
                
                ISR_HCSR04_Disable();
                if (Servo_GetPosition1()!=SCAN_LIMIT_L || Servo_GetPosition2 ()!= SERVO_LIMIT_L)
                    reset_servos();
                if (flag_connected == 1)
                {
                    start_components();
                    flag_connected  = 0;
                    sprintf (message1, "Device succesfully connected$");
                    UART_PutString(message1);
                }
                //if user selects '5' in GUI, UART sends over 'f' character and step_sweep is set to 5
                if (received == 'f')
                {
                    step_sweep = 5;
                }
                //same happens if the user selects '10'
                else if (received == 't')
                    step_sweep = 10;
                
                //reset variables but NOT step_sweep
                reset_variables();
                break;
            
            //SCAN state: the chain performs the sweep motion while the ISR activates and reads values from the  sensor
            case SCAN:
                //I don't use flags because after this line, the program gets stuck in the following for cycle
                //until the acquisition is stopped via GUI or scan is completed
                //so ISR is enabled only once
                ISR_HCSR04_Enable();
                //FIRST SWEEP: the servo starts at 40 degrees, which is the first start position, and goes on until 140 degrees
                //the increment is the step selected in the GUI
                //this for cycle goes on until the state is SCAN (second condition)
                //the increment is multiplied by the direction of the sweep, anticlockwise (LEFT, value 1) or clockwise (RIGHT, -1)
                for (angle = start_position; angle != end_position/step_sweep && state == SCAN; angle+=(step_sweep*direction))
                {
                    Servo_SetPosition1(angle);
                    //wait for servo to be in position
                    CyDelay(SWEEP_DELAY);         
                    //read the value of the angle
                    //the servos' resolution is a mechanical limitation, so we check if the angle we set (known position) and the position we read are the same
                    //because different positions could lead to wrong calculations
                    //we still give a confidence value, which is a range of (ANGLE_TOLERANCE) degrees above and below the position we set
                    //so if the servos give a wrong position that is within this range, the point is still computed as the difference is low
                    if (Servo_GetPosition1()<(angle+ANGLE_TOLERANCE) && Servo_GetPosition1()>(angle-ANGLE_TOLERANCE))
                        //find position is a function in sonar.c as it involves the USS
                        find_position();   
                    //we enter this if below when we complete a "row", so when the chain does a sweep in one direction
                    if ((direction==LEFT && angle==end_position) || (direction == RIGHT && angle == (end_position+step_sweep)) )
                    {
                        //the following is a flag that is checked in sonar.c
                        //it is reset each row, and if becomes 1 if at least one point in that row is computed
                        //if this flag is still 0 after I completed a row, I assume that there isn't an object, 
                        //or that the object scanning is complete, or that the object has been removed
                        //so if I don't find any points within the range in an entire row, scanning is considered complete
                        if (flag_print == 0)
                            state = DISPLAY;
                        //this is a function in project_utils.c that moves the chain up by 5 degrees
                        next_row();
                        flag_print = 0;
                        //swap start and end positions for the sweep
                        temp_position = end_position;
                        end_position = start_position;
                        start_position = temp_position;
                        //swap directions
                        direction = direction == LEFT ? RIGHT : LEFT;
                    }        
                }
                break;
            
            //state DISPLAY is mainly performed in the GUI
            //here we simply consider the scanning as ended so we reset everything and go back to IDLE state immediately
            //we go back to IDLE in case that the user wants to start another scan
            case DISPLAY:
                if (Servo_GetPosition1()!=SCAN_LIMIT_L || Servo_GetPosition2 ()!= SERVO_LIMIT_L)
                    reset_servos();
                reset_variables();
                step_sweep = 5;
                sprintf (message1, "Done scanning\r\n");
                UART_PutString(message1);
                state = IDLE;
                break;
            
            //state DISCONNECRED: when "Disconnect" button is pressed in the GUI, turn off components (except for UART)
            case DISCONNECTED:
                Timer_HCSR04_Stop();
                PWM_Trigger_Stop();
                Timer_TRIGGER_Stop();
                PWM_Servo1_Stop();
                PWM_Servo2_Stop();
                ISR_HCSR04_Disable();
                break;
        }      
    }
}


/* [] END OF FILE */

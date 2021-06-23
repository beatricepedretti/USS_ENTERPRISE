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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position
    if (Servo_GetPosition1()!=0 || Servo_GetPosition2 ()!= 0)
        reset_servos();
    reset_variables();
    step_sweep = 4;
 
        
    for(;;)
    {
        switch (state)
        {            
            case IDLE:
                if (Servo_GetPosition1()!=0 || Servo_GetPosition2 ()!= 0)
                    reset_servos();
                
                if (received == 'f')
                {
                    step_sweep = 5;
                }
                else if (received == 't')
                    step_sweep = 10;
                
                reset_variables();
                
                break;
            
            case SCAN:
                for (angle = start_position; angle != end_position/step_sweep && state == SCAN; angle+=(step_sweep*direction))
                {
                    Servo_SetPosition1(angle);
                    CyDelay(SWEEP_DELAY);
                    find_position();
                    if ((direction==LEFT && angle==end_position) || (direction == RIGHT && angle == (end_position+step_sweep)) )
                    {
                        next_row();
                        //end_position = (direction == LEFT) ? (SERVO_LIMIT_H - end_position) : end_position;
                        //start_position = (direction == LEFT) ? (SERVO_LIMIT_H - start_position) : start_position;
                        temp_position = end_position;
                        end_position = start_position;
                        start_position = temp_position;
                        direction = direction == LEFT ? RIGHT : LEFT;
                    }          
                }
                break;

            case DISPLAY:
                if (Servo_GetPosition1()!=0 || Servo_GetPosition2 ()!= 0)
                    reset_servos();
                reset_variables();
                step_sweep = 4;
                sprintf (message1, "Done scanning\r\n");
                UART_1_PutString(message1);
                state = IDLE;
                break;
        }      
    }
}


/* [] END OF FILE */

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

uint8_t start_position, end_position, temp_position;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position
    reset_servos();
    step_sweep = 5;
    state = WAIT;
    

    
    for(;;)
    {
        switch (state)
        {
            case WAIT:
                if (Servo_GetPosition1()!=0 || Servo_GetPosition2 ()!= 0)
                {
                    reset_servos();
                    CyDelay(SWEEP_DELAY);
                }
                step_sweep = 5;

               

                break;
            
            case IDLE:
                if (Servo_GetPosition1()!=0 || Servo_GetPosition2 ()!= 0)
                {
                    reset_servos();
                    CyDelay(SWEEP_DELAY);
                }
                if (received == 'f')
                {
                    step_sweep = 5;
                }
                else if (received == 't')
                    step_sweep = 10;
                
                start_position = SCAN_LIMIT_L;
                end_position =  SCAN_LIMIT_H;
                direction = LEFT;
                
                
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
                Servo_SetPosition1(90); //è un controllo che alla fine dovrà essere cancellato
                break;
        }      
    }
}


/* [] END OF FILE */

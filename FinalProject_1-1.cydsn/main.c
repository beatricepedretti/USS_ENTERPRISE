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
    reset_servos();
    step_sweep = 5;
    state = OFF;
    
    
    for(;;)
    {
        switch (state)
        {
            case OFF:
                reset_servos();
                
                step_sweep = 5;
                flag = 0;
                break;
            
            case IDLE:
                reset_servos();
                Servo_SetPosition1(90);
                CyDelay(500);
                flag = 0;
                if (received == 'f')
                {
                    step_sweep = 5;
                }
                else if (received == 't')
                    step_sweep = 10;
                break;
            
            case SCAN_SX:
                sweep (SERVO_LIMIT_L, (SERVO_LIMIT_H/step_sweep), SX);
                break;
                
            case SCAN_DX:
                sweep((SERVO_LIMIT_H/step_sweep),SERVO_LIMIT_L, DX);
                break;
            
            case DISPLAY:
                Servo_SetPosition1(90);
                break;
        }      
    }
}


/* [] END OF FILE */

/* ========================================
 *
 * @brief HC-SR04 Medium PSoC Project.
 *
 * This project demonstrates the usage of 
 * the HC-SR04 UltraSonic sensor with the 
 * PSoC 5LP.
 *
*/
#include "project_utils.h"


volatile uint8_t flag = 0; //flag che potrebbe segnalare a kivy che la scannerizzazione di un livello è pronta, NON so come funziona la comunicazione con kivy
volatile uint8_t clockwise=0;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position
    set_servos(0,0);
    
    for(;;)
    {
        if (clockwise == 0)
        {           
            for (angle=SERVO_LIMIT_L; angle<=(SERVO_LIMIT_H/STEP_SWEEP); angle ++)
            {
                Servo_SetPosition1(angle*STEP_SWEEP);
                if (angle == (SERVO_LIMIT_H/STEP_SWEEP))
                {                    
                    clockwise=1;
                    CyDelay(RISE_DELAY);
                    next_row();
                }
                CyDelay(SWEEP_DELAY);     
            }
        }
        if (clockwise == 1)
        {
            for (angle=(SERVO_LIMIT_H/STEP_SWEEP);angle >SERVO_LIMIT_L; angle --)
            {
                Servo_SetPosition1(angle*STEP_SWEEP);
                if (angle == (SERVO_LIMIT_L+1))
                {
                    clockwise = 0;
                    Servo_SetPosition1(SERVO_LIMIT_L);
                    CyDelay(RISE_DELAY);
                    next_row();
                    
                }
                
                CyDelay(SWEEP_DELAY);
            }
        }

    } 
 }

/* [] END OF FILE */

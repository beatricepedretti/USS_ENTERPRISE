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


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position
    set_servos(0,0);
    step_sweep = 5;
    
    /*sprintf (message, "0: %d\r\n", Servo_GetPosition2());
    UART_1_PutString(message);
    
    CyDelay(1000);
    
    Servo_SetPosition2(79);
    sprintf (message, "79: %d\r\n", Servo_GetPosition2());
    UART_1_PutString(message);
    
    CyDelay(1000);
    
    Servo_SetPosition2(180);
    sprintf (message, "180: %d\r\n", Servo_GetPosition2());
    UART_1_PutString(message);
    
    CyDelay(1000);
    
    Servo_SetPosition2(50);
    sprintf (message, "50: %d\r\n", Servo_GetPosition2());
    UART_1_PutString(message);*/
    
    
    for(;;)
    {
        switch (state)
        {
            case IDLE:
                set_servos(0,0);
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

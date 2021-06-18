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
#include "isr.h"

uint8_t STEP_SWEEP=10;
volatile uint8_t flag = 0; //flag che potrebbe segnalare a kivy che la scannerizzazione di un livello è pronta, NON so come funziona la comunicazione con kivy
volatile uint8_t clockwise=0;
volatile uint8_t get_position = 0;

char state=0;
int begin_scann=0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Init components 
    start_components();
    
    //set both servo motors to starting position
    set_servos(0,0);
    
    for(;;)
    {
        state=received;
        if(state=='b'){         
            begin_scann=1;
        }
        if(begin_scann==1){
            if(received==1 || received==5 || received==10){
                STEP_SWEEP=received;
            }
            if (clockwise == 0)
            {           
                for (angle=SERVO_LIMIT_L; angle<=(SERVO_LIMIT_H/STEP_SWEEP); angle ++)
                {
                    CyDelay(SWEEP_DELAY);
                    find_position();
                    sprintf (message, "%d %d %d\r\n", (int)X,(int)Y,(int)Z);
                    UART_1_PutString(message);
                    Servo_SetPosition1(angle*STEP_SWEEP);
                    if (angle == (SERVO_LIMIT_H/STEP_SWEEP))
                    {                    
                        clockwise=1;
                        CyDelay(RISE_DELAY);
                        next_row();
                    }
                    check_state();
                    if (begin_scann==0)
                        Servo_SetPosition1(0);
                        Servo_SetPosition2(0);
                        break;
                        //per farlo uscire dal for potremmo fare angle=180     
                }
            }
            if (clockwise == 1)
            {
                for (angle=(SERVO_LIMIT_H/STEP_SWEEP);angle >SERVO_LIMIT_L; angle --)
                {
                    CyDelay(SWEEP_DELAY);
                    find_position();
                    sprintf (message, "%d %d %d\r\n", (int)X,(int)Y,(int)Z);
                    UART_1_PutString(message);
                    Servo_SetPosition1(angle*STEP_SWEEP);
                    if (angle == (SERVO_LIMIT_L+1))
                    {
                        clockwise = 0;
                        Servo_SetPosition1(SERVO_LIMIT_L);
                        CyDelay(RISE_DELAY);
                        next_row();
                        
                    }
                    check_state();
                    if (begin_scann==0)
                        Servo_SetPosition1(0);
                        Servo_SetPosition2(0);
                        break;
                        //per farlo uscire dal for potremmo fare angle=180
                }
            }
        }
    }
}

/* [] END OF FILE */

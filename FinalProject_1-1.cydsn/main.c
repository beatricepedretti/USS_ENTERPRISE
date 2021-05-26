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


#ifndef HIGH
    #define HIGH 1u
#endif

#ifndef LOW
    #define LOW 0U
#endif

#define PI 3.14159265
#define array_length 100
#define STEP_ANGLE 10


double val = PI / SERVO_LIMIT_H;

int i = 0; //indice per salvare coordinate dei punti 3D negli array
int flag = 0; //flag che potrebbe segnalare a kivy che la scannerizzazione di un livello è pronta, NON so come funziona la comunicazione con kivy

double pos_servo1_rad;
double pos_servo2_rad;

uint16_t pos_servo1;
uint16_t pos_servo2;
uint8_t angle;
uint8_t angle_2;

uint8_t d_2; //di quanto l'aggancio del servo2 è spostata rispetto all'albero del servo1
uint8_t Z1 = 30; //altezza albero servo2 in mm, quindi altezza servo1 + metà spessore servo2. Ho messo 30 a caso
uint8_t aggancio_sonar = 4;//da misurare

float x_array[array_length];
float y_array[array_length];
float z_array[array_length];

float X;
float Y;
float Z;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Init components 
    
    Timer_HCSR04_Start();
    PWM_Trigger_Start();
    Timer_TRIGGER_Start();
    UART_1_Start();
    PWM_Servo1_Start();
    PWM_Servo2_Start();
    
    // Call the Custom_ISR_Start function -- defined in isr.c
    Custom_ISR_Start();
    /* Send message over UART */
    UART_1_PutString("HC-SR04 Program Started\r\n");
    Servo_SetPosition1(SERVO_LIMIT_L);
    Servo_SetPosition2(SERVO_LIMIT_L); //DA VEDERE, PROBABILMENTE PI/2
    angle_2=SERVO_LIMIT_L;
    
    for(;;)
    {
       
            /* Generate 10uS pulse on Trigger PIN 
                _
            ___| |___________________________________
            
            The rising edge of the trigger pin will reset the counter.
            The rising and falling edge of the echo pin will trigger and
            force a capture of the timer, respectively. The capture event
            will trigger the ISR_HCSR04 defined in the isr.c source file.
            */
        
   
        
        for (angle=0;angle<=(SERVO_LIMIT_H/STEP_ANGLE);angle++)
        {

            Servo_SetPosition1(angle*STEP_ANGLE);
            
            CyDelay(500);
            
            pos_servo1 = Servo_GetPosition1(); 
            pos_servo2 = Servo_GetPosition2(); 

            //queste sarebbero le formule date da cerveri..
            //X=sin()*cos()*distance; 
            //Y=cos()*cos()*distance;
            //Z=sin()*distance;

            //CONSIDERO L'ANGOLO IN RADIANTI PER FUNZIONE SIN E COS
            pos_servo1_rad=(pos_servo1-SERVO_MID_ANGLE)*val;
            pos_servo2_rad=(pos_servo2-SERVO_MID_ANGLE)*val;
            /*
            sprintf(message_1, "coord: %d mm\r\n", (int)pos_servo1_rad);
            UART_1_PutString(message_1);
            */
            //CALCOLO COORDINATE PUNTO INDIVIDUATO
            X = sin(pos_servo1_rad)*(d_2+(distance+aggancio_sonar)); //-90 se consideriamo lo zero di pos_servo1 l'angolo totalmente a sinistra (180 gradi)
            Z = Z1+(distance+aggancio_sonar)*sin(pos_servo2_rad); //-90 sempre se consideriamo lo zero l'angolo totalmente a sinistra (quello che farà scendere il sonar di altezza)
            Y = (d_2+distance+aggancio_sonar)*cos(pos_servo1_rad); 
           
            /*
            sprintf(message_1, "coord: %d mm\r\n", (int)Z);
            UART_1_PutString(message_1);
            */

            
            /*x_array[i]=X;
            y_array[i]=Y;
            z_array[i]=Z;
            i++;
                */
            
            
            //CONTROLLO ACQUISIZIONE 180 GRADI, CAMBIO LIVELLO DI SCANNERIZZAZIONE
            
            if(angle==(SERVO_LIMIT_H/STEP_ANGLE)){ 
               /* memset(x_array,0,array_length);
                memset(y_array,0,array_length);
                memset(z_array,0,array_length);
                */
                angle_2=angle_2+STEP_ANGLE;
                Servo_SetPosition2(angle_2);
                if (angle_2==SERVO_LIMIT_H)
                    angle_2=SERVO_LIMIT_L; //?
                flag=1;
                i=0;
                angle=SERVO_LIMIT_L;
            }
            flag=0;
        }
    } 
 }

/* [] END OF FILE */

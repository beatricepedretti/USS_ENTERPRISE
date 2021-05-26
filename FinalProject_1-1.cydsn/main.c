/* ========================================
 *
 * @brief HC-SR04 Medium PSoC Project.
 *
 * This project demonstrates the usage of 
 * the HC-SR04 UltraSonic sensor with the 
 * PSoC 5LP.
 *
*/
#include "isr.h"
#include "project.h"
#include "stdio.h"
#include "servo.h"


#ifndef HIGH
    #define HIGH 1u
#endif

#ifndef LOW
    #define LOW 0U
#endif

#define PI 3.14159265
double val = PI / 180;

int i = 0; //indice per salvare coordinate dei punti 3D negli array
int flag = 0; //flag che potrebbe segnalare a kivy che la scannerizzazione di un livello è pronta, NON so come funziona la comunicazione con kivy

double pos_servo1_rad;
double pos_servo2_rad;
uint8_t pos_servo1;
uint8_t pos_servo2;
uint8_t angle;
uint8_t angle_2;

uint8_t d_2; //di quanto l'aggancio del servo2 è spostata rispetto all'albero del servo1
uint8_t Z1 = 30; //altezza albero servo2 in mm, quindi altezza servo1 + metà spessore servo2. Ho messo 30 a caso
uint8_t aggancio_sonar = 4;//da misurare

float x_array[18];
float y_array[18];
float z_array[18];

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
    Servo_SetPosition1(0);
    Servo_SetPosition2(0); //DA VEDERE, PROBABILMENTE PI/2
    angle_2=0;
    
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
            
        
        for (angle=0;angle<=18;angle++)
        {

            Servo_SetPosition1(angle*10);
            
            /*ControlReg_TRIGGER_Write(HIGH);
            CyDelayUs(10);
            ControlReg_TRIGGER_Write(LOW);
            CyDelay(1000);
            */
            CyDelay(1100);
            
            pos_servo1 = Servo_GetPosition1(); 
            pos_servo2 = Servo_GetPosition2(); 

            //queste sarebbero le formule date da cerveri..
            //X=sin()*cos()*distance; 
            //Y=cos()*cos()*distance;
            //Z=sin()*distance;
            
            //CONSIDERO L'ANGOLO IN RADIANTI PER FUNZIONE SIN E COS
            pos_servo1_rad=(pos_servo1-90)*val;
            pos_servo2_rad=(pos_servo2-90)*val;
            
            //CALCOLO COORDINATE PUNTO INDIVIDUATO
            X = sin(pos_servo1_rad)*(d_2+(distance+aggancio_sonar)); //-90 se consideriamo lo zero di pos_servo1 l'angolo totalmente a sinistra (180 gradi)
            Z = Z1+(distance+aggancio_sonar)*sin(pos_servo2_rad); //-90 sempre se consideriamo lo zero l'angolo totalmente a sinistra (quello che farà scendere il sonar di altezza)
            Y = (d_2+distance+aggancio_sonar)*cos(pos_servo1_rad); 
           
            /*sprintf(message_1, "Coord: %d mm\r\n", X);
            UART_1_PutString(message_1);
            */
            
            x_array[i]=X;
            y_array[i]=Y;
            z_array[i]=Z;
            i++;
            
            //CONTROLLO ACQUISIZIONE 180 GRADI, CAMBIO LIVELLO DI SCANNERIZZAZIONE
            
            if(angle==18){ 
                angle_2=angle_2+10;
                Servo_SetPosition2(angle_2);
                if (angle_2==180)
                    angle_2=0; //?
                flag=1;
                i=0;
                angle=0;
            }
            flag=0;
        }
    }
}

/* [] END OF FILE */

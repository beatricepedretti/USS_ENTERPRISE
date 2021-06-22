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


#ifndef HIGH
    #define HIGH 1u
#endif

#ifndef LOW
    #define LOW 0u
#endif

void find_position ()
{
    //this function compute the coordinates of a three dimensional point 
    //thanks to the position returned by the two servos and the distance returned 
    //by the USS
    
    float32 pos_servo1_rad;
    float32 pos_servo2_rad;
    uint16_t distance_real;
    uint16_t distance;
    uint8_t i;
    
    //conversion from degrees to rad
    pos_servo1_rad=(SERVO_MID_ANGLE-Servo_GetPosition1())*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(SERVO_MID_ANGLE-Servo_GetPosition2())*(PI / SERVO_LIMIT_H);
    
    for (i=0, distance=0; i<3; i++)
    {
        distance+=compute_distance();
        CyDelay(100);
    }

    //computation of coordinates
    distance_real = distance/i;
    
    x=D2*(sin(pos_servo1_rad))+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    y=D2*cos(pos_servo1_rad)+(distance_real+ARM_LENGTH)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    z=Z1+(distance_real+ARM_LENGTH)*sin(pos_servo2_rad);
    sprintf (message, "%d %d %d\r\n", (int)x,(int)y,(int)z);
    UART_1_PutString(message);
    CyDelay(100);
}

uint16_t compute_distance()
{
    int timer_count_begin = 0;  // Timer count value @ begin 
    int timer_count_end = 0;    // Timer count value @ end
    int duration = 0;
    /* Generate 10uS pulse on Trigger PIN 
            _
        ___| |___________________________________
        */
        Pin_TRIGGER_Write(HIGH);
        CyDelayUs(10);
        Pin_TRIGGER_Write(LOW);        
        /* Wait until Echo PIN is low */
        while(Pin_ECHO_Read() == LOW);
        /* Read Timer value when Echo PIN becomes HIGH */
        timer_count_begin = Timer_HCSR04_ReadCounter();
        /* Wait until Echo PIN remains high 
                         ______
        ________________|      |_________________
        */
        while(Pin_ECHO_Read());
        /* Read Timer value when Echo PIN becomes LOW */
        timer_count_end = Timer_HCSR04_ReadCounter();
        /* Compute the duration */
        if (timer_count_end > timer_count_begin)
            duration = (Timer_HCSR04_ReadPeriod() + timer_count_begin) - timer_count_end;
        else
            duration = timer_count_begin - timer_count_end;
        /* Prepare the string and send it over the UART */
        return (duration/CONV_FACTOR*10);
}



/* [] END OF FILE */

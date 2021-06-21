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

void find_position ()
{
    //this function compute the coordinates of a three dimensional point 
    //thanks to the position returned by the two servos and the distance returned 
    //by the USS
    
    float32 pos_servo1_rad;
    float32 pos_servo2_rad;
    
    //conversion from degrees to rad
    pos_servo1_rad=(SERVO_MID_ANGLE-Servo_GetPosition1())*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(SERVO_MID_ANGLE-Servo_GetPosition2())*(PI / SERVO_LIMIT_H);

    //computation of coordinates    
    x=D2*(sin(pos_servo1_rad))+(ARM_LENGTH+distance)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    y=D2*cos(pos_servo1_rad)+(distance+ARM_LENGTH)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    z=Z1+(distance+ARM_LENGTH)*sin(pos_servo2_rad);
    sprintf (message, "%d %d %d\r\n", (int)x,(int)y,(int)z);
    UART_1_PutString(message);
}

/* [] END OF FILE */

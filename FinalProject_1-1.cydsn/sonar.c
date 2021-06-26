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
    
    double pos_servo1_rad;
    double pos_servo2_rad;
    double pos_servo2_z_rad;
    int16_t distance_real;
    
    //conversion from degrees to rad
    pos_servo1_rad= (double)(SERVO_MID_ANGLE-Servo_GetPosition1())*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(double)(Servo_GetPosition2()+ANGLE_ZERO-SERVO_MID_ANGLE)*(PI / SERVO_LIMIT_H);
    pos_servo2_z_rad=(double)(Servo_GetPosition2()+ANGLE_ZERO)*(PI / SERVO_LIMIT_H);

    //computation of coordinates
    distance_real = distance;
    
    //if the point is within a certain distance
    if(distance_real<DISTANCE_TH_H && distance_real>DISTANCE_TH_L)
    {     
        y=(D2+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*cos(pos_servo1_rad);
        x=(D2+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*sin(pos_servo1_rad);
        z=Z1-(ARM_LENGTH+distance_real)*cos(pos_servo2_z_rad);
        //send coordinates to UART
        sprintf (message, "%d %d %d\r\n", (int16_t)x,(int16_t)y,(int16_t)z);
        UART_1_PutString(message);
        //if at least one point is found in the row, set this flag to 1
        flag_print = 1;  
    }
}
/* [] END OF FILE */

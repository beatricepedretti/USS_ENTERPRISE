/* =========================================================
 *
 * U.S.S. E.N.T.E.R.P.R.I.S.E.
 * Ultrasound Sensor for ENvironmenT Recording and
 * Point-cloud Reconstruction Integrated Scanning Equipment
 * 
 * Electronic Technologies and Biosensors Laboratory
 * Final Project
 * 
 * Di Liddo, Goshen, Pedretti
 *
 * =========================================================
*/

#include "project_utils.h"

void find_position ()
{
    //this function computes the coordinates of a three dimensional point 
    //thanks to the position returned by the two servos and the distance returned 
    //by the USS
    
    double pos_servo1_rad;
    double pos_servo2_rad;
    double pos_servo2_z_rad;
    int16_t distance_real;
    double x, y, z; //coordinates
    
    //conversion from degrees to rad
    pos_servo1_rad= (double)(SERVO_MID_ANGLE-Servo_GetPosition1())*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(double)(Servo_GetPosition2()+ANGLE_ZERO-SERVO_MID_ANGLE)*(PI / SERVO_LIMIT_H);
    pos_servo2_z_rad=(double)(Servo_GetPosition2()+ANGLE_ZERO)*(PI / SERVO_LIMIT_H);

    //setting distance_real = distance because distance is computed in ISR and may change during computation of coordinates
    // by setting distance_real = distance and using only distance_real we keep one value of distance for the computation
    distance_real = distance;

    //if the point is within a certain distance
    if (distance_real<DISTANCE_TH_H && distance_real>DISTANCE_TH_L)
    {
        z=(double)(Z1-(double)(ARM_LENGTH+distance_real)*cos(pos_servo2_z_rad));
        //negative zs are artifacts, so I check
        //if the z is negative, the point is neglected
        if (z>0)
        {
            //compute the other coordinates here to save CPU
            y=(double)((D2+(double)(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*cos(pos_servo1_rad));
            x=(double)((D2+(double)(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*sin(pos_servo1_rad));            
            //send coordinates to UART
            sprintf (message, "%d %d %d\r\n", (int16_t)x,(int16_t)y,(int16_t)z);
            UART_PutString(message);
            //if at least one point is found in the row, set this flag to 1
            flag_print = 1;  
        }
    }
}
/* [] END OF FILE */

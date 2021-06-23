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
    float32 pos_servo2_z_rad;
    int16_t distance_real;
    
    //conversion from degrees to rad
    pos_servo1_rad=(SERVO_MID_ANGLE-Servo_GetPosition1())*(PI / SERVO_LIMIT_H);
    pos_servo2_rad=(Servo_GetPosition2()+ANGLE_ZERO-SERVO_MID_ANGLE)*(PI / SERVO_LIMIT_H);
    pos_servo2_z_rad=(Servo_GetPosition2()+ANGLE_ZERO)*(PI / SERVO_LIMIT_H);

    //computation of coordinates
    distance_real = distance_acc/(distance_count+1);
    
    //x=D2*(sin(pos_servo1_rad))+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad)*sin(pos_servo1_rad);
    //y=D2*cos(pos_servo1_rad)+(distance_real+ARM_LENGTH)*cos(pos_servo2_rad)*cos(pos_servo1_rad);
    //z=Z1+(distance_real+ARM_LENGTH)*sin(pos_servo2_rad);
    
     /*   
        y=(D2+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*cos(pos_servo1_rad);
        
    if(y<DISTANCE_TH)
    {   
        x=(D2+(ARM_LENGTH+distance_real)*cos(pos_servo2_rad))*sin(pos_servo1_rad);
        z=Z1-(ARM_LENGTH+distance_real)*cos(pos_servo2_z_rad);
        sprintf (message, "%d %d %d\r\n", (int16_t)x,(int16_t)y,(int16_t)z);
        UART_1_PutString(message);
    }
    */
    //sprintf (message1, "Average: %d\r\n", (int16_t)distance_real);
    //UART_1_PutString(message1);
    distance_acc = 0;
    distance_count = 0;
}
/* [] END OF FILE */

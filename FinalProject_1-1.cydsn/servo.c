/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


#include "project_utils.h"



/**
 *  \brief          Map value in new interval.
 *  \param[in]      x: the value to be remapped.
 *  \param[in]      in_min: minimum value for input interval.
 *  \param[in]      in_max: maximum value for input interval.
 *  \param[out]     out_min: minimum value for output interval.
 *  \param[out]     out_max: maximum value for output interval.
 *  \return         the remapped value.
 */
uint16_t Servo_Map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    
}

/**
 *  \brief          Set the servo to a new angle.
 *  \param[in]      deg: the degree angle to be set.
 *  \return         PWM compare value correspondent to the angle.
 */
uint16_t Servo_SetPosition1(uint16_t deg) {
 
    // Get the compare value for the desired position
    float tmp = Servo_Map(deg, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    
    // Update the PWM compare value
    uint16_t compareValue = round(tmp);
    PWM_Servo1_WriteCompare(compareValue);
    
    return compareValue;
    
}

uint16_t Servo_SetPosition2(uint16_t deg) {
 
    // Get the compare value for the desired position
    float tmp = Servo_Map(deg, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    
    // Update the PWM compare value
    uint16_t compareValue = round(tmp);
    PWM_Servo2_WriteCompare(compareValue);
    
    return compareValue;
    
}

/**
 *  \brief          Get the current position of the servo.
 *  \return         The servo motor angle.
 */
uint16_t Servo_GetPosition1(void) {
 
    // Get the actual compare value
    uint16_t comparePWM = PWM_Servo1_ReadCompare();
    
    // Map the value to degrees
    return Servo_Map(comparePWM, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H, SERVO_LIMIT_L, SERVO_LIMIT_H);
    
}

uint16_t Servo_GetPosition2(void) {
 
    // Get the actual compare value
    uint16_t comparePWM = PWM_Servo2_ReadCompare();
    
    // Map the value to degrees
    return Servo_Map(comparePWM, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H, SERVO_LIMIT_L, SERVO_LIMIT_H);
    
}

/**
 *  \brief          Perform motion profile with servo motor.
 *  
 *  This function will sweep all the angles between \ref xi and \ref xf
 *  with a delay of \ref t between the two consecutive positions.
 *
 *  \param[in]      xi: the initial angle position.
 *  \param[in]      xf: the final angle position.
 *  \param[in]      t: the time between two consecutive positions.
 *  \note           This is a blocking function.
 */
/*void Servo_MotionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t) {
    
    // Check : current position = xi ?
    if ( Servo_GetPosition() != xi ) {
        Servo_SetPosition(xi);
    }
 
    // Sleep time [ms]
    uint16_t dt = abs(xf-xi)/t;
    
    // Direction
    int8_t dx = (xf > xi) ? 1 : -1;
    
    while( xi != xf ) {
    
        xi += dx;
        Servo_SetPosition(xi);
        CyDelay(dt);
        
    } 
    
    // Force final position --> ?
    Servo_SetPosition(xf);
    
}*/

/* [] END OF FILE */


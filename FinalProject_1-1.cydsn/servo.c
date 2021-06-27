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

//This function sets both servo motors to start position
void reset_servos (void)
{
    Servo_SetPosition2(SERVO_LIMIT_L);
    CyDelay(SWEEP_DELAY);
    Servo_SetPosition1(SCAN_LIMIT_L);
    CyDelay(SWEEP_DELAY);    
}


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

/* [] END OF FILE */


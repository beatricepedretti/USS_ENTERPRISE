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

/**
 *  \file           servo.c
 *  \brief          Source file for servo motor interface.
 *
 *  \author         Davide Marzorati, Mattia Pesenti
 */

/*******************************************************************************
 * Copyright (c) 2021 Marzorati Davide, Pesenti Mattia
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include "servo.h"

#include "math.h"
#include "PWM_Servo1.h"
#include "PWM_Servo2.h"
#include "stdio.h"
#include "stdlib.h"

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
uint16_t Servo_SetPosition(uint16_t deg) {
 
    // Get the compare value for the desired position
    float tmp = Servo_Map(deg, SERVO_LIMIT_L, SERVO_LIMIT_H, SERVO_PWM_LIMIT_L, SERVO_PWM_LIMIT_H);
    
    // Update the PWM compare value
    uint16_t compareValue = round(tmp);
    PWM_Servo1_WriteCompare(compareValue);
    
    return compareValue;
    
}

/**
 *  \brief          Get the current position of the servo.
 *  \return         The servo motor angle.
 */
uint16_t Servo_GetPosition(void) {
 
    // Get the actual compare value
    uint16_t comparePWM = PWM_Servo1_ReadCompare();
    
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
void Servo_MotionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t) {
    
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
    
}

/* [] END OF FILE */




/* [] END OF FILE */

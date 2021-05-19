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
 *  \file           servo.h
 *  \brief          Header file for servo motor interface.
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

#ifndef __SERVO_H
    #define __SERVO_H
    
    #ifdef __cpluscplus
        extern "C" {
    #endif
    
    #include <cytypes.h>
        
    
    /*************************************
     *          API Constants            *
     ************************************/

    /**
     *  \brief          Lowest servo angle.
     */
    #define SERVO_LIMIT_L   0

    /**
     *  \brief          Highest servo angle.
     */
    #define SERVO_LIMIT_H   180

    /**
     *  \brief          PWM Compare value for \ref SERVO_LIMIT_L angle.
     */
    #define SERVO_PWM_LIMIT_L   1547

    /**
     *  \brief          PWM Compare value for \ref SERVO_LIMIT_H angle.
     */
    #define SERVO_PWM_LIMIT_H   1399
    
    uint16_t Servo_Map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
    uint16_t Servo_SetPosition1(uint16_t deg);
    
    uint16_t Servo_SetPosition2(uint16_t deg);
    uint16_t Servo_GetPosition1(void);
    uint16_t Servo_GetPosition2(void);
    void Servo_MotionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t);
    
    #ifdef __cpluscplus
        extern }
    #endif
#endif

/* [] END OF FILE */




/* [] END OF FILE */

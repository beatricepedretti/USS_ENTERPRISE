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

#ifndef __SERVO_H
    #define __SERVO_H
    
    #ifdef __cpluscplus
        extern "C" {
    #endif
    
    #include "project_utils.h"
        
    
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
     *  \brief          Middle servo angle.
     */
    
    #define SERVO_MID_ANGLE 90
    

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
    
    /*typedef struct{
        double X; 
        double Y;
        double Z;
    } Pos;
    */
    
    void Servo_MotionProfileBlocking(uint16_t xi, uint16_t xf, uint8_t t);
    
    #ifdef __cpluscplus
        extern }
    #endif
#endif

/* [] END OF FILE */




/* [] END OF FILE */

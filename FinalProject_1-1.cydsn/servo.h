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

#ifndef __SERVO_H
    #define __SERVO_H
    
    #ifdef __cpluscplus
        extern "C" {
    #endif
    
    #include "project_utils.h"
    #include "PWM_Servo1.h"
    #include "PWM_Servo2.h"    
    
    /*************************************
     *          API Constants            *
     ************************************/

    //Lowest scanning limit 
    #define SCAN_LIMIT_L 40
    
    //Highest scanning limit
    #define SCAN_LIMIT_H 140
    
    //Lowest servo angle
    #define SERVO_LIMIT_L   0

    //Highest servo angle
    #define SERVO_LIMIT_H   180
    
    //Offset servo2
    #define ANGLE_ZERO 60
    
    //Middle servo angle
    #define SERVO_MID_ANGLE 90
    
    //PWM Compare value for \ref SERVO_LIMIT_L angle
    #define SERVO_PWM_LIMIT_L   1547

    //PWM Compare value for \ref SERVO_LIMIT_H angle
    #define SERVO_PWM_LIMIT_H   1399
    
    void reset_servos (void);
    
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
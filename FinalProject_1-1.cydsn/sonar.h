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
#ifndef __SONAR_H
    #define __SONAR_H

    #include "Timer_HCSR04.h"
    #ifndef HIGH
        #define HIGH 1u
    #endif

    #ifndef LOW
        #define LOW 0u
    #endif
    
    #define SAMPLES_TO_AVG 3
    #define CONV_FACTOR 58.25

    #define PI 3.14159265
    #define D2 9 //distance between servo1's drive shaft and servo2's drive shaft
    #define Z1 113 //servo2 drive shaft height from plane
    #define ARM_LENGTH 53 //distance between USS and servo2 drive shaft
    #define DISTANCE_TH_H 500
    #define DISTANCE_TH_L 70
    
    
    void find_position(void);
    
#endif

/* [] END OF FILE */

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
    

    #define CONV_FACTOR 58.25

    #define PI 3.14159265
    #define D2 13 //distance between servo1's drive shaft and servo2's drive shaft
    #define Z1 73 //servo2 drive shaft height from plane
    #define ARM_LENGTH 57 //distance between USS and servo2 drive shaft

    void find_position(void);
    uint16_t compute_distance(void);
    
#endif

/* [] END OF FILE */

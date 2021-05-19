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
 * @file isr.h
 * @brief Header file for ISR functions.
 * 
 * This header file contains some function definitions
 * to be used with the ISR component.
 * 
 * @author Davide Marzorati
 * @date March 28, 2019
 */

#ifndef ISR_H
    #define ISR_H

    #include "cytypes.h"
    
    
    // Message to send over UART
    char message[30];
    // Timer period value
    int timer_period;
    
    /**
    * @brief Start the ISR component.
    *
    * This function starts the ISR component and stores
    * the period of the timer to be used when converting
    * time to distance.
    *
    * @return Nothing
    */
    void Custom_ISR_Start(void);
    
    /**
    * @brief ISR routine.
    *
    * This ISR is used to compute the distance starting
    * from a measurement of time.
    */
    CY_ISR_PROTO(ISR_ULTRASONIC);

#endif

/* [] END OF FILE */


/* [] END OF FILE */

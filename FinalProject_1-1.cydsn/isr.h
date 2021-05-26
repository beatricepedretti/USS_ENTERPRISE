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



#ifndef ISR_H
    #define ISR_H

    #include "cytypes.h"
    
    #include "math.h"
    
    
    // Message to send over UART
    uint16_t distance;
    char message[30];
    //char message_1[30];
    // Timer period value
    uint16_t timer_period;
    
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



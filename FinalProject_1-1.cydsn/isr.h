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

    #include "project_utils.h"
    
    
    
    // Message to send over UART
    char received;
    int distance;
    char message[STRING_LENGTH];
    char message_1[STRING_LENGTH];

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
    CY_ISR_PROTO(Custom_UART_RX_ISR);
    

#endif

/* [] END OF FILE */



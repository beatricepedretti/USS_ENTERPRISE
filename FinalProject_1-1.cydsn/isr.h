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

#ifndef ISR_H
    #define ISR_H

    #include "project_utils.h"
    
    // Message to send over UART
    //uint8_t state;
    uint16_t received;
    uint16_t distance;
    uint8_t distance_count;
    uint distance_acc;
    
    

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



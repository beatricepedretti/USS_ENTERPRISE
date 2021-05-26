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

#include "project_utils.h"
/**
* @brief Conversion factor from time to mm.
*/
#ifndef CONV_FACTOR
    #define CONV_FACTOR 58.25
#endif

#ifndef TO_MM
    #define TO_MM 10.00
#endif


void Custom_ISR_Start(void) {
    // Store the value of the timer
    timer_period = Timer_HCSR04_ReadPeriod();
    // Start the ISR component
    ISR_HCSR04_StartEx(ISR_ULTRASONIC); 
}

CY_ISR(ISR_ULTRASONIC){
    // Compute distance and send it over uart
    distance=(uint)((timer_period - Timer_HCSR04_ReadCapture())/CONV_FACTOR*TO_MM);
    sprintf(message, "Distance: %d mm\r\n", distance);
    UART_1_PutString(message);
    
}


/* [] END OF FILE */

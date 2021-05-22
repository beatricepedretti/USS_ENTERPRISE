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

#include "isr.h"
#include "stdio.h"
#include "UART_1.h"
#include "Timer_Sonar.h"
#include "isr_Sonar.h"

/**
* @brief Conversion factor from time to mm.
*/
#ifndef CONV_FACTOR
    #define CONV_FACTOR 58.25
#endif




void Custom_ISR_Start(void) {
    // Store the value of the timer
    timer_period = Timer_Sonar_ReadPeriod();
    // Start the ISR component
    isr_Sonar_StartEx(ISR_ULTRASONIC); 
}

CY_ISR(ISR_ULTRASONIC){
    // Compute distance and send it over uart
    distance=(uint8_t)((timer_period - Timer_Sonar_ReadCapture())/58.0*10);
    sprintf(message, "Distance: %d mm\r\n", distance);
    UART_1_PutString(message);
}


/* [] END OF FILE */

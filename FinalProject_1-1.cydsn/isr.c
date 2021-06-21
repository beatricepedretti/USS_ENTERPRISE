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
    //find_position();
    //sprintf(message, "%d\r\n", distance);
    //sprintf (message, "%d %d %d\r\n", (int)X,(int)Y,(int)Z);
    //UART_1_PutString(message);
    
}
CY_ISR(Custom_UART_RX_ISR) 
{ 
    // We read the received data, and echo it back
    if (UART_1_ReadRxStatus() == UART_1_RX_STS_FIFO_NOTEMPTY) 
    {
        received = UART_1_ReadRxData();
        if(received=='v'){  
            //if that allows the automatical UART connection through pc and psoc
            
            sprintf (message1, "Device succesfully connected$");
            UART_1_PutString(message1);
            state = IDLE;
        }
        if (received == 'b'){
            state = SCAN;
        }
        if (received == 's'){
            state = IDLE;
        }
    }
}

/* [] END OF FILE */

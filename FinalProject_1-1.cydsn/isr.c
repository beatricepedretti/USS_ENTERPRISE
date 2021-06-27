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

//Conversion factor from time to mm.
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
    // Compute distance   
    distance=(uint16_t)((timer_period - Timer_HCSR04_ReadCapture())/CONV_FACTOR*TO_MM);  
}

CY_ISR(Custom_UART_RX_ISR) 
{ 
    // We read the received data, and echo it back
    if (UART_1_ReadRxStatus() == UART_1_RX_STS_FIFO_NOTEMPTY) 
    {
        received = UART_1_ReadRxData();
        if(received=='v'){  
            //if that allows the automatical UART connection through pc and psoc
            flag_connected = 1;
        }
        //when user selects START in GUI
        if (received == 'b'){
            state = SCAN;
        }
        //when user selects STOP 
        if (received == 's'){
            state = IDLE;
        }
    }
}

/* [] END OF FILE */

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
#include "project.h"
#include "stdio.h"
#include "servo.h"
#include "UART_1.h"
#include "Timer_HCSR04.h"
#include "ISR_HCSR04.h"
#include "cytypes.h"
#include "math.h"
#include "PWM_Servo1.h"
#include "PWM_Servo2.h"
#include "stdlib.h"

#ifndef HIGH
    #define HIGH 1u
#endif

#ifndef LOW
    #define LOW 0U
#endif

#define PI 3.14159265
#define COORD_ARRAY_LENGTH 100
#define STEP_SWEEP 2
#define STEP_RISE 5
#define RISE_LIMIT 90
#define STRING_LENGTH 30

uint8_t angle;
uint8_t angle_2;



void start_components(void);
void set_servos (uint8_t, uint8_t);
void next_row (void);



/* [] END OF FILE */

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
#include "sonar.h"

#ifndef HIGH
    #define HIGH 1u
#endif

#ifndef LOW
    #define LOW 0U
#endif

#define PI 3.14159265
#define COORD_ARRAY_LENGTH 100
#define STEP_RISE 5
#define RISE_LIMIT 90
#define STRING_LENGTH 100
#define RISE_DELAY 500
#define SWEEP_DELAY 600
#define DISTANCE_TH 300
#define SIMPLE_SWEEP 0
#define OBJECT_SHAPER 1
#define D2 13 //distance between servo1's drive shaft and servo2's drive shaft
#define Z1 73 //servo2 drive shaft height from plane
#define ARM_LENGTH 57 //distance between USS and servo2 drive shaft
#define IDLE 0
#define SCAN_SX 1
#define SCAN_DX 2
#define DISPLAY 3
#define SX 0
#define DX 1

uint8_t state;
uint8_t angle;
uint8_t angle_2;
uint8_t direction;
uint8_t step_sweep;
uint8_t msg_ready;


float x, y, z;


void start_components();
void set_servos (uint8_t, uint8_t);
void next_row (void);
void find_position(void);
void sweep (uint8_t, uint8_t, uint8_t);

/* [] END OF FILE */

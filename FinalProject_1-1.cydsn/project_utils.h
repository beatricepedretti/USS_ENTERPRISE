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

#include "isr.h"
#include "project.h"
#include "stdio.h"
#include "servo.h"
#include "UART_1.h"
#include "cytypes.h"
#include "math.h"
#include "stdlib.h"
#include "sonar.h"

#define STEP_RISE 5
#define RISE_LIMIT 60
#define STRING_LENGTH 100
#define RISE_DELAY 500
#define SWEEP_DELAY 600

#define WAIT 0
#define IDLE 1
#define SCAN 2
#define DISPLAY 3

#define LEFT 1 //anticlockwise direction
#define RIGHT -1 //clockwise direction

volatile uint8_t state;
uint16_t angle;
uint16_t angle_2;
int8_t direction;
volatile uint8_t step_sweep;
uint8_t start_position, end_position, temp_position;

char message[STRING_LENGTH];
char message1[STRING_LENGTH];

float32 x, y, z;

void start_components(void);
void next_row (void);
void reset_variables (void);

/* [] END OF FILE */

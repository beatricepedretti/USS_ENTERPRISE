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

//this file includes all tools of the project
//for example, here we have all the includes for all header files so that in further .c and .h files we can only import "project_utils.h"

//we have here the defines, variables and functions that regulate the movement of the cinematic chain

#include "isr.h"
#include "project.h"
#include "stdio.h"
#include "servo.h"
#include "UART_1.h"
#include "cytypes.h"
#include "math.h"
#include "stdlib.h"
#include "sonar.h"

#define STEP_RISE 5 //step by which the chain moves in vertical direction
#define RISE_LIMIT 60 //end of the rise
#define STRING_LENGTH 100
#define RISE_DELAY 500
#define SWEEP_DELAY 600

//states
#define IDLE 0
#define SCAN 1
#define DISPLAY 2

#define LEFT 1 //anticlockwise direction
#define RIGHT -1 //clockwise direction

volatile uint8_t state;
volatile uint8_t flag_print; //check if at least one value has been printed in a row
uint16_t angle_2;
int8_t direction;
volatile uint8_t step_sweep;
uint8_t start_position, end_position;

char message[STRING_LENGTH];
char message1[STRING_LENGTH];

double x, y, z; //coordinates

void start_components(void);
void next_row (void);
void reset_variables (void);

/* [] END OF FILE */

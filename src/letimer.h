/*
 * @filename 	: letimer.h
 * @description	: contains headers to support letimer.c
 * @author		: Puneet Bansal
 */

#include <em_letimer.h>
#include "em_core.h"
#include "em_cmu.h"
#include <math.h>
#include "gpio.h"
#include <stdint.h>

uint32_t ticks,freq,LedOn_Ticks,flags;
uint8_t motion_debounce_flag;
uint8_t magnetic_debounce_flag;

/***********************************************************************************************
 * Function name: letimer_init()                       	                                       *
 * Description: Populating LETIMER_Init structure with default values and initialising LETIMER.*
 * Set the compare value in COMP0 register according to the required delay using CompareSet(). *
 * Enable underflow interrupt and LETIMER.                                                     *
 * @param: None                                                                                *
 * @return: None					                                                           *
 ***********************************************************************************************/

void letimer_init();

/***********************************************************************************************
 * Function name: timerWaitUs()                       	                                       *
 * Description:  Function to generate blocking delay in micro seconds.                         *
 * @param: delay in microsecond required                                                       *
 * @return: None					                                                           *
 ***********************************************************************************************/
void timerWaitUs(uint32_t);


/***********************************************************************************************
 * Function name: timerSetEventinms()                       	                               *
 * Description:  triggers a comp1 interrupt on achieving the desired amount of ms delay.       *
 * @param: delay in microsecond required                                                       *
 * @return: None					                                                           *
 ***********************************************************************************************/
void timerSetEventinms(uint32_t);

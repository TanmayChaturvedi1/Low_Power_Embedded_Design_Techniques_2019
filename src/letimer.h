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

void prescale_set();
void letimer_init();
void timerWaitUs(uint32_t);
void timerSetEventinms(uint32_t);

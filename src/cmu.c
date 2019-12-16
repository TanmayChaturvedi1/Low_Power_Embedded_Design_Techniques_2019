/*
 * @filename	: cmu.c
 * @description	: This file contains functions to configure clocks.
 * @author 		: Puneet Bansal, Tanmay Chaturvedi, Nachiket Kelkar
 * @references  : Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 *
 */

#include "cmu.h"
#include "main.h"

void clock_init()
{
	if(sleepEM>=0 && sleepEM<3)
	{
		CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);			//cmuOsc_HFXO Enable low frequency crystal oscillator
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); 		//cmuOsc_HFXO Selecting the branch as LFA and clock source to be LFXO
	}
	else if(sleepEM==3)
	{
		CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);			//Enable low frequency crystal oscillator
	}

	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockEnable(cmuClock_LETIMER0,true);
}

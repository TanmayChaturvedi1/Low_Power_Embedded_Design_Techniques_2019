/*
 * @filename	: letimer.c
 * @description	: This file contains functions to configure LETIMER
 * @author 		: Puneet Bansal, Nachiket Kelkar , Tanmay Chaturvedi
 *
 */
#include "letimer.h"
#include "main.h"
int i=1;


void letimer_init()
{

	LETIMER_Init_TypeDef init = LETIMER_INIT_DEFAULT;

	init.enable=false;				//start counting when init completed
	init.comp0Top=true;				//load comp0 value in CNT on underflow
	init.debugRun=false;			//not running counter in debug mode
	init.repMode=letimerRepeatFree;	//count until stopped
	init.out0Pol=0;
	init.out1Pol=0;
	init.ufoa0=letimerUFOANone;
	init.ufoa1=letimerUFOANone;

	LETIMER_Init(LETIMER0,&init);
	LETIMER_CompareSet(LETIMER0,0,10000);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF ); /*Enable Underflow interrupts*/

	LETIMER_Enable(LETIMER0, true);
}



void timerWaitUs(uint32_t us_wait)
{
	uint32_t current_ticks,max_tick,count_upto;
	uint32_t us_ticks;

	us_ticks=freq * us_wait;
	us_ticks=us_ticks/1000000;	/*Calculate the ticks required in us*/

	current_ticks=LETIMER_CounterGet(LETIMER0); /*Get the present value of timer count*/

	if(current_ticks>us_ticks)
	{
		count_upto=current_ticks-us_ticks;
		while(LETIMER_CounterGet(LETIMER0)!=count_upto);
	}
	else
	{
		max_tick=LETIMER_CompareGet(LETIMER0,0);
		while(LETIMER_CounterGet(LETIMER0)!=(max_tick-(us_ticks-current_ticks)));
	}
}

void timerSetEventinms(uint32_t ms_until_wakeup)
{
	uint32_t current_ticks=0;
	uint32_t max_tick,count_upto;
	uint32_t ms_ticks;

	ms_ticks=1000 * ms_until_wakeup;
	ms_ticks=ms_ticks/1000;	/*Calculate the ticks required in s*/

	current_ticks=LETIMER_CounterGet(LETIMER0); /*Get the present value of timer count*/

	if(current_ticks>=ms_ticks)
	{
		count_upto=current_ticks-ms_ticks;
	}
	else
	{
		max_tick=LETIMER_CompareGet(LETIMER0,0);
		count_upto=(max_tick-(ms_ticks-current_ticks));
	}

	LETIMER_CompareSet(LETIMER0,1,count_upto);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);
}


/***********************************************************************************************
 * Function name: LETIMER0_IRQHandler()                       	                               *
 * Description:  Interrupt handler for LETIMER0                                                *
 * @param: None                                                                                *
 * @return: None					                                                           *
 ***********************************************************************************************/

void LETIMER0_IRQHandler(void)
{
CORE_ATOMIC_IRQ_DISABLE ();
flags=LETIMER_IntGetEnabled(LETIMER0);
LETIMER_IntClear(LETIMER0,flags);
//LOG_INFO("LETIMER triggered");
if((flags & LETIMER_IF_UF) == LETIMER_IF_UF)
	{
	scheduler |= TIMESTAMP_1SEC;
	}

if((flags & LETIMER_IF_COMP1) == LETIMER_IF_COMP1)
{
	if(inside_motion == 1)
	{
		motion_debounce_flag =1 ;
	}
	if(inside_magnetic == 1)
	{
		magnetic_debounce_flag =1 ;
	}

	LETIMER_IntDisable(LETIMER0,LETIMER_IFC_COMP1);
}
CORE_ATOMIC_IRQ_ENABLE();
}


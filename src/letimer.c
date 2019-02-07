/*
 * @filename	: letimer.c
 * @description	: This file contains functions to configure LETIMER
 * @author 		: Puneet Bansal
 *
 */
#include "letimer.h"
#include "main.h"
int i=1;

/*
 * @description
 * Populating LETIMER_Init structure with default values and initialising LETIMER.
 * Set the compare value in COMP0 register according to the required delay using CompareSet().
 * Enable underflow interrupt, NVIC and LETIMER.
 */
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

	prescale_set();

	LETIMER_Init(LETIMER0,&init);

	LETIMER_CompareSet(LETIMER0,0,ticks);

	//LedOn_Ticks= freq*LED_ONTIME;
	//LETIMER_CompareSet(LETIMER0,1,LedOn_Ticks);

	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF ); /*Enable Underflow interrupts*/

	NVIC_EnableIRQ(LETIMER0_IRQn);

	LETIMER_Enable(LETIMER0, true);
}

/*
 * @description
 * Set the prescaler value depending on the selected energy mode and the desired
 * LED period.
 */

void prescale_set()
{
	int j=0;
	if(sleepEM>=0 && sleepEM<3)
	{
		freq= LFX0_FREQ;		/*Select freq=32768 for EM(0-2)*/
	}
	else if (sleepEM==3)
	{
		freq= ULFRCO_FREQ;		/*Select freq=1000Hz for EM(3)*/
	}
	ticks=LED_PERIOD*freq;

	if(ticks> MAX_TICKS)
	{
		while(ticks> MAX_TICKS)
		{
			ticks=ticks/2;
			freq=freq/2;
			j++;
		}
		CMU_ClockDivSet(cmuClock_LETIMER0,pow(2,j));
	}
}
/*
 * @description
 *  Function to generate delay in micro seconds.
 *
 * @param us_wait
 * delay in microsecond required
 *
 */
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
		//gpioLed0SetOn();
		while(LETIMER_CounterGet(LETIMER0)!=count_upto);
		//gpioLed0SetOff();
	}
	else
	{
		max_tick=LETIMER_CompareGet(LETIMER0,0);

		//gpioLed0SetOn();
		while(LETIMER_CounterGet(LETIMER0)!=(max_tick-(us_ticks-current_ticks)));
		//gpioLed0SetOff();

	}
}


/*
 * @description
 * Interrupt handler for LETIMER0. Checks for the source of interrupts. If the interrupt
 * source is COMP1 match, then LED is turned on.
 * If the interrupt source is Underflow Interrupt, then LED is turned off
 *
 */

void LETIMER0_IRQHandler(void)
{
CORE_ATOMIC_IRQ_DISABLE ();
flags=LETIMER_IntGet(LETIMER0);

scheduler=MEAS_TEMP;
LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);


CORE_ATOMIC_IRQ_ENABLE();
}


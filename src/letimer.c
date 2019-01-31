/*
 * @filename	: letimer.c
 * @description	: This file contains functions to configure LETIMER
 * @author 		: Puneet Bansal
 *
 */
#include "letimer.h"
#include "main.h"


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

	LedOn_Ticks= freq*LED_ONTIME;
	LETIMER_CompareSet(LETIMER0,1,LedOn_Ticks);

	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1| LETIMER_IEN_UF); /*Enable COMP1 and Underflow interrupts*/

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
if(EM>=0 && EM<3)
	{
	freq= LFX0_FREQ;		/*Select freq=32768 for EM(0-2)*/
	}
else if (EM==3)
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
 * Interrupt handler for LETIMER0. Checks for the source of interrupts. If the interrupt
 * source is COMP1 match, then LED is turned on.
 * If the interrupt source is Underflow Interrupt, then LED is turned off
 *
 */

void LETIMER0_IRQHandler(void)
{
CORE_ATOMIC_IRQ_DISABLE ();
flags=LETIMER_IntGet(LETIMER0);

if(flags & LETIMER_IF_COMP1)
	{
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
	gpioLed0SetOn();
	}
if(flags & LETIMER_IF_UF)
	{
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
	gpioLed0SetOff();
	}
CORE_ATOMIC_IRQ_ENABLE();
}


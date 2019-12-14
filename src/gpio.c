/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>
#include "log.h"

/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port gpioPortC
#define LED0_pin 6
#define LED1_port gpioPortC
#define LED1_pin 7

extern int scheduler;
extern docking_mode dock_button_status;


void gpioInit()
{
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	GPIO_PinModeSet(MOTION_DETECTION_PORT, MOTION_DETECTION_PIN, gpioModeInputPull, 1);
	GPIO_PinModeSet(MAGNETIC_DETECTION_PORT, MAGNETIC_DETECTION_PIN, gpioModeInputPull, 1);
	//GPIO_PinModeSet(gpioPortF, 6, gpioModeInputPull, 1);


	GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1);//DEBUG GPIO
	GPIO_DriveStrengthSet(gpioPortF, gpioDriveStrengthStrongAlternateStrong);

	GPIO_PinModeSet(gpioPortF, 3, gpioModePushPull, 1); //TO TURN ON THE SENSOR
	GPIO_PinModeSet(gpioPortA,1, gpioModePushPull,1);//ERROR DEBUG.


//	GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true, true );
//	GPIO_ExtIntConfig(MAGNETIC_DETECTION_PORT,MAGNETIC_DETECTION_PIN, MAGNETIC_DETECTION_PIN , false, true, true );

	//GPIO_ExtIntConfig(gpioPortF,6,6, true, true,true );

//	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
//	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void gpioLed0Toggle()
{
	GPIO_PinOutToggle(LED0_port,LED0_pin);
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

uint8_t readGPIOPin(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin )
{
	uint8_t val = GPIO_PinInGet(gpioPort,gpioPin);
	return val;
}



/* Function to turn the NFC ON/OFF */
void NFC_on(bool on)
{
	if(on)
	{
		GPIO_DriveStrengthSet(NFC_POWER_PORT, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinModeSet(NFC_POWER_PORT, NFC_POWER_PIN, gpioModePushPull, false);
		GPIO_PinOutSet(NFC_POWER_PORT, NFC_POWER_PIN);
	}
	else
	{
		GPIO_PinOutClear(NFC_POWER_PORT, NFC_POWER_PIN);
	}
}


/* Enable/Disable docking mode */
// Docking mode interrupts
void docking_mode_config()
{
	GPIO_PinModeSet(DOCKING_PORT,DOCKING_PIN,gpioModeInput,false);
	GPIO_ExtIntConfig(	DOCKING_PORT,
						DOCKING_PIN,
						DOCKING_INT_NO,
						RISING_EDGE_INT_ENABLE,
						FALLING_EDGE_INT_ENABLE,
						DOCKING_INTERRUPTS_ENABLE);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}


/* Get current mode of the docking switch */
docking_mode get_docking_switch_position()
{
	return GPIO_PinInGet(DOCKING_PORT, DOCKING_PIN);
}


void GPIO_EVEN_IRQHandler()
{
	LOG_INFO("%d",GPIO_IntGetEnabled());
	GPIO_IntClear(GPIO_IntGetEnabled());
	LOG_INFO("Entered handler");
	//GPIO_PinOutSet(gpioPortD,10);
	scheduler |= MOTION_DETECTION;
	//gotInt=1;
}

void GPIO_ODD_IRQHandler()
{
	//LOG_INFO("Entered odd handler");
	uint32_t flags = GPIO_IntGetEnabled();
	GPIO_IntClear(flags);
	LOG_INFO("flags= %d", flags);
	LOG_INFO("Entered handler");
	if((flags & (1<<5)) == (1<<5))
	{
		LOG_INFO("Inside magnetic detection");
		scheduler |= MAGNETIC_DETECTION;
	}
	if((flags & (1<<13)) == (1<<13))
	{
	LOG_INFO("Inside switch detection");
	int pos = get_docking_switch_position();
		if(pos)
		{
			dock_button_status = BUTTON_TRANSIT;
		}
		else
		{
			dock_button_status = BUTTON_DOCK;
		}
	}
}

//void gpioCallback1(uint8_t pin)
//{
//	LOG_INFO("Entered handler");
//}


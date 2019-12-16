/*
 * @filename 	: gpio.h
 * @description	: contains headers to support gpio.c
 * @author		: Puneet Bansal, Nachiket Kelkar and Tanmay Chaturvedi
 * @reference   : Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 */


#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdint.h>
#include <stdbool.h>
#include <em_gpio.h>

typedef enum{
	BUTTON_DOCK,
	BUTTON_TRANSIT,
	BUTTON_NONE
}docking_mode;

/***************************************************************************************
 * Function name: gpioInit()                                                 	       *
 * Description: Function initializes GPIO pins with the mode and drive strength        *
 * @param: void                                                                        *
 * @return: void                                                                       *
 ***************************************************************************************/
void gpioInit();

/***************************************************************************************
 * Function name: readGPIOPin()                                                 	   *
 * Description: Function reads the value of the GPIO pin and returns it                *
 * @param: void                                                                        *
 * @return: 8 bit value of the gpio                                                    *
 ***************************************************************************************/
uint8_t readGPIOPin(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin);

/***************************************************************************************
 * Function name: NFC_on()                                                 	           *
 * Description: Function to turn the NFC module On/Off                                 *
 * @param: 1 or 0 to indicate on/off respectively                                      *                            *
 * @return: None					                                                   *
 ***************************************************************************************/
void NFC_on(bool);

/***************************************************************************************
 * Function name: docking_mode_config()                                    	           *
 * Description: Function to configure the gpio pin for docking mode                    *
 * @param: None                                     *                                  *
 * @return: None					                                                   *
 ***************************************************************************************/
void docking_mode_config();

/***************************************************************************************
 * Function name: get_docking_switch_position()                            	           *
 * Description: Function to get the state of the docking switch                        *
 * @param: None                                     *                                  *
 * @return: None					                                                   *
 ***************************************************************************************/
docking_mode get_docking_switch_position();


/***************************************************************************************
 * Function name: gpioLed0SetOn()                       	                           *
 * Description: Function to LED0 on                                                    *
 * @param: None                                                                        *
 * @return: None					                                                   *
 ***************************************************************************************/
void gpioLed0SetOn();

/***************************************************************************************
 * Function name: gpioLed0SetOff()                       	                           *
 * Description: Function to LED0 off                                                   *
 * @param: None                                                                        *
 * @return: None					                                                   *
 ***************************************************************************************/
void gpioLed0SetOff();

/***************************************************************************************
 * Function name: gpioLed1SetOn()                       	                           *
 * Description: Function to LED1 on                                                    *
 * @param: None                                                                        *
 * @return: None					                                                   *
 ***************************************************************************************/
void gpioLed1SetOn();

/***************************************************************************************
 * Function name: gpioLed0SetOn()                       	                           *
 * Description: Function to LED0 on                                                    *
 * @param: None                                                                        *
 * @return: None					                                                   *
 ***************************************************************************************/
void gpioLed1SetOff();



#define GEEKY_PANDA_GPIO 1
#define MAGNETIC_DETECTION (1<<1)
#define MOTION_DETECTION (1<<2)

#if GEEKY_PANDA_GPIO
#define MOTION_DETECTION_PORT gpioPortF //INT1
#define MOTION_DETECTION_PIN 4

#define MAGNETIC_DETECTION_PORT gpioPortF //INT2
#define MAGNETIC_DETECTION_PIN 5

#else
#define MOTION_DETECTION_PORT gpioPortA //INT1
#define MOTION_DETECTION_PIN 3

#define MAGNETIC_DETECTION_PORT gpioPortD //INT2
#define MAGNETIC_DETECTION_PIN 12

#endif


#define NFC_SWITCH_port gpioPortC
#define NFC_SWITCH_pin 8


// NFC defines
#define NFC_POWER_PORT				gpioPortC
#define NFC_POWER_PIN				8

// The docking mode defines
#define DOCKING_PORT 				gpioPortB
#define DOCKING_PIN					13
#define DOCKING_INT_NO				13
#define FALLING_EDGE_INT_ENABLE 	true
#define RISING_EDGE_INT_ENABLE		true
#define FALLING_EDGE_INT_DISABLE	false
#define RISING_EDGE_INT_DISABLE		false
#define DOCKING_INTERRUPTS_ENABLE	true
#define DOCKING_INTERRUPTS_DISABLE	false

#define TRANSIT_MODE	(1 << 3)
#define DOCKING_MODE 	(1 << 4)


#endif /* SRC_GPIO_H_ */

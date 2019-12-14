/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
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

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
uint8_t readGPIOPin(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin);
void NFC_on(bool);
void docking_mode_config();
docking_mode get_docking_switch_position();

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




int gotInt,gotInt1;
#endif /* SRC_GPIO_H_ */

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

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
uint8_t readGPIOPin(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin);

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


int gotInt,gotInt1;
#endif /* SRC_GPIO_H_ */

/* @FileName	: main.h
 * @description	: contains header files and #defines for the project.
 * @author 		: Puneet Bansal
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 */

#include <stdint.h>
#include "cmu.h"
#include "letimer.h"
#include "i2c.h"
#include <stdlib.h>
#include <stdbool.h>
#include "log.h"
#include "si7021.h"

#define sleepEM (3) 		/*Energy mode you want to sleep in. i.e. if EM = 2 then, EM3,EM4 will be blocked.*/
#define LED_PERIOD (3)		/*Total LED period in seconds*/
#define LED_ONTIME (.175)	/*LED on time in seconds*/
#define LFX0_FREQ (32768)	/*Low frequency crystal oscillator frequency*/
#define ULFRCO_FREQ (1000)	/*Ultra low frequency crystal oscillator frequency*/
#define MAX_TICKS (65535)	/*Maximum ticks possible for a 16bit timer*/
#define MEAS_TEMP (1<<0)

#define SENSOR_ENABLE_PIN 15
#define SENSOR_ENABLE_PORT gpioPortD

volatile uint16_t scheduler; 		/*Global variable to keep a track of events*/






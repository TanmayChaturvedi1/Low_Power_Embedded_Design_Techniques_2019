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

#define sleepEM (1) 		/*Energy mode you want to sleep in. i.e. if EM = 2 then, EM3,EM4 will be blocked.*/
#define LED_PERIOD (3)		/*Total LED period in seconds*/
#define LED_ONTIME (.175)	/*LED on time in seconds*/
#define LFX0_FREQ (32768)	/*Low frequency crystal oscillator frequency*/
#define ULFRCO_FREQ (1000)	/*Ultra low frequency crystal oscillator frequency*/
#define MAX_TICKS (65535)	/*Maximum ticks possible for a 16bit timer*/
#define MEAS_TEMP (1<<0)

#define SENSOR_ENABLE_PIN 15
#define SENSOR_ENABLE_PORT gpioPortD

#define TIMESTAMP_1SEC 1

/*DEFINE to switch between loading the code into blue gecko and loading the code on the
GeekyPanda board.
In the GeekyPanda board, DC-DC converter and external oscillator is turned off and the internal
oscillator is always turned on.
*/
#define GEEKY_PANDA_LOAD 1
//also change in i2c.h
//and gpio.h

volatile uint16_t scheduler; 		/*Global variable to keep a track of events*/
uint8_t temp_ready;
uint32_t timestamp_var;
uint8_t inside_motion;
uint8_t inside_magnetic;
/*
 */

typedef enum
{
SYSTEM_DOC_SRC,
SYSTEM_TRANSIT,
SYSTEM_DOC_DESTN
}stateOfSystem;

stateOfSystem systemState;
stateOfSystem prevState;
uint8_t tempThreshold;
uint8_t humidThreshold;

void task_scheduler();
void interrupt_enable();
void dockingMode_initialisations();
void transitMode_initialisations();
void interrupt_disable();


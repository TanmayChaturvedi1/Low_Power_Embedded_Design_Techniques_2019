/* @FileName	: main.h
 * @description	: contains header files and #defines for the project and function definitions used in main.c.
 * @author 		: Puneet Bansal, Tanmay Chaturvedi and Nachiket Kelkar
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 */

#include <stdint.h>
#include "cmu.h"
#include "letimer.h"
#include "i2c.h"
#include <stdlib.h>
#include <stdbool.h>

#include "FXOS8700CQ.h"
#include "log.h"

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

#define sleepEM (2) 		/*Energy mode you want to sleep in. i.e. if EM = 2 then, EM3,EM4 will be blocked.*/
#define LED_PERIOD (3)		/*Total LED period in seconds*/
#define LED_ONTIME (.175)	/*LED on time in seconds*/
#define LFX0_FREQ (32768)	/*Low frequency crystal oscillator frequency*/
#define ULFRCO_FREQ (1000)	/*Ultra low frequency crystal oscillator frequency*/
#define MAX_TICKS (65535)	/*Maximum ticks possible for a 16bit timer*/


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

/*@param: none
 *@return : none
 *@description:  Task scheduler that takes care of the events raised through the interrupts. Following are the interrupts that
 * can occur in our system :
 * 10 second letimer timeout
 * motion and magnetic detection interrupt
 */
void task_scheduler();

/*@param: none
 *@return : none
 *@description: Enables Letimer and GPIO interrupts in the system
*/
void interrupt_enable();

/*@param: none
 *@return : none
 *@description : Initialise nfc to operate in docking mode.
 */
void dockingMode_initialisations();

/*@param : none
 *@return : none
 *@description : Initialises Letimer, i2c, SPI and all the sensors
 */
void transitMode_initialisations();

/*@param : none
 *@return : none
 *@description : Disbales all the interrupt so that the system can be in docking mode.
 */
void interrupt_disable();


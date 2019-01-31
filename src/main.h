/* @FileName	: main.h
 * @description	: contains header files and #defines for the project.
 * @author 		: Puneet Bansal
 *
 */

#include <stdint.h>
#include "cmu.h"
#include "letimer.h"
#include <stdlib.h>
#include <stdbool.h>


#define EM (2) 				/*Energy mode you want to sleep in. i.e. if EM = 2 then, EM3,EM4 will be blocked.*/
#define LED_PERIOD (2.25)	/*Total LED period in seconds*/
#define LED_ONTIME (.175)	/*LED on time in seconds*/
#define LFX0_FREQ (32768)	/*Low frequency crystal oscillator frequency*/
#define ULFRCO_FREQ (1000)	/*Ultra low frequency crystal oscillator frequency*/
#define MAX_TICKS (65535)	/*Maximum ticks possible for a 16bit timer*/


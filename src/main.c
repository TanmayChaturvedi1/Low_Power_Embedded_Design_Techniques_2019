/*
 * @filename	: main.c
 * @description	: Configuring LETIMER0 in various sleep modes and blinking LED at
 * 				  appropriate LED on time.
 * @author 		: Puneet Bansal
 * @Reference	: Lecture 4 slides
 */


/* Board headers */
#include "main.h"
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

//#define LED_period
//#define LED_ontime

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "em_device.h"
#include "em_chip.h"
#include "gpio.h"
#include "sleep.h"
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];


// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


int main(void)
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Initialize stack
  gecko_init(&config);

  clock_init();

  letimer_init();
  gpioInit();



  /* Infinite loop */
  if(EM>0 && EM<3) 				//Check if desired energy mode is 1,2
  SLEEP_SleepBlockBegin(EM+1);


  while (1)
  {
	 //em_sleep();
	  if(EM>0 && EM<3 )			//If the desired energy mode is EM1, EM2 use the SLEEP_Sleep function
	  SLEEP_Sleep();
	  else if(EM==3)
	  EMU_EnterEM3(true);		//If the desired energy mode is EM3 use EMU_EnterEM3 function.

  }
}



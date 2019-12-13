/*
 * @filename	: main.c
 * @description	: Enabling I2C communication between Si7021 and blue gecko to obtain temperature readings.
 *
 * @author 		: Puneet Bansal
 * @Reference	: Lecture 5&6 slides
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
#include "spi_routines.h" //for temp sensor
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

/*DEFINE to switch between loading the code into blue gecko and loading the code on the
GeekyPanda board.
In the GeekyPanda board, DC-DC converter and external oscillator is turned off and the internal
oscillator is always turned on.
*/
//#define GEEKY_PANDA_LOAD 1
extern struct bme280_dev dev;
extern struct bme280_data comp_data;

uint8_t second_count;
int main(void)
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Initialize stack
  //gecko_init(&config);
  //LOG_INFO("log initialised");

  clock_init();
  //LOG_INFO("clock initialised");
  letimer_init();
  //LOG_INFO("letimer initialised");
  gpioInit();
  //LOG_INFO("GPIO initialised");
  GPIO_IntClear(0xffff);

  logInit();
  LOG_INFO("log initialised");



  i2c_init();
  LOG_INFO("i2c initialised");

  BME280_SPI_init();
  LOG_INFO("spi initialised");
  /*Spi varibales*/

  int8_t result;
  uint8_t reg_data;
  temp_humidity_return_status_e return_status;
  temp_ready=0;
  TxBufferIndex = 0;
  RxBufferIndex = 0;
  timer_expired = false;

  if(sleepEM>0 && sleepEM<3) 							//Check if desired energy mode is 1,2
  SLEEP_SleepBlockBegin(sleepEM+1);

  //freefall_detection();
  gotInt =0;
  gotInt1 = 0;
  GPIO_PinOutClear(gpioPortD,10);
  GPIO_PinOutSet(gpioPortF,3);//TO TURN ON THE SENSOR
  timerWaitUs(1000); // time delay to ensure the sensor turns on
  LOG_INFO("Before check func");
  magnetic_detection();
  motion_detection();
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  //LOG_INFO("Entered main");
  /*Temp sensor BIST*/
  /*Temp sensor initialisation*/
  result = BME280_Device_init();
  second_count =0;
  scheduler =0;


  if( result == BME280_OK)
    {
  	  LOG_INFO("Sensor Initialized\n");
  	  bme280_get_regs(BME280_CHIP_ID_ADDR, &reg_data, 1, &dev);
  	  temp_ready = 1;
    }

  LETIMER_Enable(LETIMER0, true);
  while (1)
  {
	  task_scheduler();

  }//ending while

}//ending main

void task_scheduler()
{
	if((scheduler & TIMESTAMP_1SEC) == TIMESTAMP_1SEC)
		  	  {
		  		  LOG_INFO("Entered scheduler");
		  		  second_count++;
		  		  if(second_count%5 == 0)
		  		  {
		  			  return_status = get_temp_pres_humidity(25, 50);
		  		  	  LOG_INFO("Temp sensor return %d",return_status);
		  		  	  second_count = 0;
		  		  }
		  	  scheduler &= ~ TIMESTAMP_1SEC;
		  	  }

		  if((scheduler & MOTION_DETECTION) == MOTION_DETECTION)
		  {
			  uint8_t source = i2c_write_read(0X0C,1);
			  LOG_INFO("source in motion %x",source);
			  if((source & 0x04) ==0x04)
			  {
				  GPIO_PinOutSet(gpioPortD,10);

				  LOG_INFO("Inside If");
				  uint8_t val = i2c_write_read(0x16,1);
			  }
			  scheduler &= ~ MOTION_DETECTION;
		  }

		  if((scheduler & MAGNETIC_DETECTION) == MAGNETIC_DETECTION)
		  	{
			  uint8_t source1 = i2c_write_read(0X0C,1);
			  uint8_t source = i2c_write_read(0X53,1);
		  		  LOG_INFO("Source of interrupt is %x", source);
		  		  //GPIO_PinOutSet(gpioPortD,10);
		  		  if((source & 0x04) ==0x04)
		  		  {
		  			  LOG_INFO("Inside If");
		  			  //uint8_t val = i2c_write_read(0x16,1);
		  			  magXMax = magYMax = magZMax = (int16_t)0x8000;
					  magXMin = magYMin = magZMin = (int16_t)0x7FFF;
		  		  }
		  		scheduler &= ~ MAGNETIC_DETECTION;

		  	  }


}


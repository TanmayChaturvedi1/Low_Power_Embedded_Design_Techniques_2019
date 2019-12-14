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
#include "nfc_data.h"
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];
extern uint8_t nfc_data[117][16];

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
sensor_return_status return_status;
docking_mode package_status;

extern uint8_t sector;
docking_mode dock_button_status;
int main(void)
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  clock_init();

  //letimer_init();

  gpioInit();

  GPIO_IntClear(0xffff);

  logInit();

  //BME280_SPI_init();
  LOG_INFO("After bme init");
//  /*Spi varibales*/
//
//  int8_t result;
//  uint8_t reg_data;
//
//  temp_ready=0;
//  TxBufferIndex = 0;
//  RxBufferIndex = 0;
  timer_expired = false;
  motion_debounce_flag= 1;
  magnetic_debounce_flag = 1;
  inside_motion = 0;
  inside_magnetic = 0;
  temp_threshold = 25;
  humid_threshold = 50;
  sector = 0x02;

  if(sleepEM>0 && sleepEM<3) 			//Check if desired energy mode is 1,2
  SLEEP_SleepBlockBegin(sleepEM+1);

  gotInt =0;
  gotInt1 = 0;
  GPIO_PinOutClear(gpioPortD,10);
  GPIO_PinOutSet(gpioPortF,3);			//TO TURN ON THE SENSOR
  timerWaitUs(1000); 					// time delay to ensure the sensor turns on

  //magnetic_detection();
  //motion_detection();

  //NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  //NVIC_EnableIRQ(GPIO_ODD_IRQn);

  /*Temp sensor initialisation*/

  second_count =0;
  scheduler =0;
  timestamp_var =0;
//  result = BME280_Device_init();
//  LOG_INFO("Before sensor intialisation");
//  if( result == BME280_OK)
//    {
//  	  LOG_INFO("Sensor Initialized\n");
//  	  bme280_get_regs(BME280_CHIP_ID_ADDR, &reg_data, 1, &dev);
//  	  temp_ready = 1;
//    }
  LOG_INFO("After sensor initialisation");
  tempThreshold =0;
  humidThreshold =0;
  systemState = SYSTEM_DOC_SRC;
  prevState = SYSTEM_DOC_SRC;

  docking_mode_config();
  dock_button_status = get_docking_switch_position();
  uint8_t transit_initialisations = 1;
  LOG_INFO("button status is", dock_button_status);


  while (1)
  {
	  switch(systemState)
	  {
	  case(SYSTEM_DOC_SRC):

				if(dock_button_status == BUTTON_DOCK)
				{
					LOG_INFO("Inside system doc");
					dockingMode_initialisations();
					nfc_get_all_the_written_values(NFC_SLAVE_ADD,&tempThreshold, &humidThreshold);
					LOG_INFO("temp = %d, humid = %d", tempThreshold,humidThreshold);
					systemState = SYSTEM_TRANSIT;
					prevState =	SYSTEM_DOC_SRC;
					dock_button_status = BUTTON_NONE;
				}
				else
				{
					//LOG_INFO("Change the switch to Docking Mode");
				}
	  break;

	  case(SYSTEM_TRANSIT):
		//LOG_INFO("Inside system transit");
				if(((prevState == SYSTEM_DOC_SRC)| (prevState == SYSTEM_TRANSIT)) && (dock_button_status == BUTTON_TRANSIT))
				{
					if(transit_initialisations == 1)
					{
						transitMode_initialisations();
						interrupt_enable();
						transit_initialisations = 0;
					}
					task_scheduler();
					prevState = SYSTEM_TRANSIT;
					systemState = SYSTEM_TRANSIT;
				}

				else if(prevState == SYSTEM_DOC_SRC && dock_button_status == BUTTON_NONE)
					{
						systemState = SYSTEM_TRANSIT;
					}
				else if(prevState == SYSTEM_TRANSIT && dock_button_status == BUTTON_DOCK)
					{
						dock_button_status = BUTTON_NONE;
						systemState = SYSTEM_DOC_DESTN;
					}


	  break;

	  case(SYSTEM_DOC_DESTN):
		//LOG_INFO("Inside system doc destination");
			  interrupt_disable();
	  	  	  dockingMode_initialisations();
	  	  	  for(int i =1;i<3;i++)
	  	  	  {
	  	  		  for(int j=0;j<16;j++)
	  	  		  {
	  	  			  printf("%c ",nfc_data[i][j]);
	  	  		  }
	  	  		 printf("\n");
	  	  	  }
	  	  	  nfc_i2c_write_data_to_nfc(NFC_SLAVE_ADD, sector);
	  	  	  prevState = SYSTEM_DOC_DESTN;
	  	  	  systemState = SYSTEM_TRANSIT;
	  	  	  //LOG_INFO("Reached the end of the cycle");
	  	  	  //systemState = SYSTEM_DOC_SRC;
	  	  	  break;

	  }
  }//ending while


}//ending main

void task_scheduler()
{
	if((scheduler & TIMESTAMP_1SEC) == TIMESTAMP_1SEC)
	{
		LOG_INFO("Entered scheduler");
		second_count++;
		timestamp_var++;
		if(second_count%10 == 0)
		{
			return_status = get_temp_pres_humidity(tempThreshold, humidThreshold);
			LOG_INFO("Temp sensor return %d",return_status);
			nfc_data_update(timestamp_var,return_status);
			second_count = 0;
			GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true, true );
			GPIO_ExtIntConfig(MAGNETIC_DETECTION_PORT,MAGNETIC_DETECTION_PIN, MAGNETIC_DETECTION_PIN , false, true, true );
		}
		scheduler &= ~ TIMESTAMP_1SEC;
	}

	if((scheduler & MOTION_DETECTION) == MOTION_DETECTION)
	{
		{
			GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true, false );
			uint8_t source = i2c_write_read(0X0C,1);
			if((source & 0x04) ==0x04)
			{
				GPIO_PinOutSet(gpioPortD,10);
				LOG_INFO("Inside motion detection If");
				nfc_data_update(timestamp_var,EVENT_MOTION_DETECTED);
				uint8_t val = i2c_write_read(0x16,1);
			}
		}
		scheduler &= ~ MOTION_DETECTION;
		inside_motion = 0;
	}

	if((scheduler & MAGNETIC_DETECTION) == MAGNETIC_DETECTION)
	{
		{
			GPIO_ExtIntConfig(MAGNETIC_DETECTION_PORT,MAGNETIC_DETECTION_PIN, MAGNETIC_DETECTION_PIN , false, true, false );
			uint8_t source1 = i2c_write_read(0X0C,1);
			uint8_t source = i2c_write_read(0X53,1);
			//LOG_INFO("Source of interrupt is %x", source);
			//GPIO_PinOutSet(gpioPortD,10);
			if((source & 0x04) ==0x04)
			{
				LOG_INFO("Inside Magnetic Detection If");
				nfc_data_update(timestamp_var,EVENT_MAGNETIC_FIELD_DETECTED);

			}
			scheduler &= ~ MAGNETIC_DETECTION;
		}

	}


}
void dockingMode_initialisations()
{
	 nfc_i2c_init();
}

void transitMode_initialisations()
{
	letimer_init();
	i2c_init();
	BME280_SPI_init();
	magnetic_detection();
	motion_detection();
	temp_sensor_init();
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void interrupt_enable()
{
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF ); /*Enable Underflow interrupts*/
	GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true, true );
	GPIO_ExtIntConfig(MAGNETIC_DETECTION_PORT,MAGNETIC_DETECTION_PIN, MAGNETIC_DETECTION_PIN , false, true, true );
	NVIC_EnableIRQ(LETIMER0_IRQn);

}

void interrupt_disable()
{
	LETIMER_IntDisable(LETIMER0, LETIMER_IEN_UF); /*Enable Underflow interrupts*/
	GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true, false);
	GPIO_ExtIntConfig(MAGNETIC_DETECTION_PORT,MAGNETIC_DETECTION_PIN, MAGNETIC_DETECTION_PIN , false, true, false);
	NVIC_DisableIRQ(LETIMER0_IRQn);
}

/*
 * @filename	: main.c
 * @description	: Configuring system state machines to operate in Docking source,docking destination and transit mode.
 * 				  Performing i2c, spi, nfc and sensor intializations.
 * @author 		: Puneet Bansal , Nachiket Kelkar , Tanmay Chaturvedi
 * @Reference	: Silicon Labs SDK : https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 * 				  Reference Manual : https://www.silabs.com/documents/login/reference-manuals/bluetooth-le-and-mesh-software-api-reference-manual.pdf
 *				  Assignments developed for the course IoT Embedded Firmware.
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

	//clock initialisations for letimer
	clock_init();

	//gpio pin initialisations
	gpioInit();

	GPIO_IntClear(0xffff);

	logInit();
	LOG_INFO("ASSET HEALTH ASSESSMENT AND TRACKING");

	/*Global variable initializations*/
	timer_expired = false;
	motion_debounce_flag= 1;
	magnetic_debounce_flag = 1;
	inside_motion = 0;
	inside_magnetic = 0;
	temp_threshold = 25;
	humid_threshold = 50;
	sector = 0x02;
	second_count =0;
	scheduler =0;
	timestamp_var =0;
	tempThreshold =0;
	humidThreshold =0;
	systemState = SYSTEM_DOC_SRC;
	prevState = SYSTEM_DOC_SRC;
	LOG_INFO("Basic Initializations Complete");

	/*Configuring sleep modes. The mode you want the system to sleep in can be changeg from main.h*/
	if(sleepEM>0 && sleepEM<3) 			//Check if desired energy mode is 1,2
		SLEEP_SleepBlockBegin(sleepEM+1);

	GPIO_PinOutClear(gpioPortD,10);

	/*Turn on Accelerometer & Magnetometer Sensor*/
	GPIO_PinOutSet(gpioPortF,3);

	/* Time delay to ensure the sensor turns on */
	timerWaitUs(1000);

	/*Configuring docking pin interrupts and getting the state of the switch at startup*/
	docking_mode_config();
	dock_button_status = get_docking_switch_position();
	uint8_t transit_initialisations = 1;
	uint8_t printOnce = 1;
	//LOG_INFO("button status is", dock_button_status);


	while (1)
	{	/*State Machine to switch between DOCKING_SOURCE, TRANSIT, DOCKING_DESTINATION. The states change whenever
   	there is an event from switching the docking switch*/
		switch(systemState)
		{
		/*All Interrupts Disabled. Only NFC I2C is initialized to read the time stamp,
		 * humidity and temperature threshold from NFC. */
		case(SYSTEM_DOC_SRC):

								if(dock_button_status == BUTTON_DOCK)
								{
									//LOG_INFO();
									LOG_INFO("********** STATE : DOCKING SOURCE, SWITCH : DOCK **********");
									dockingMode_initialisations();
									/*Function to fetch the temperature and humidity threshold from NFC*/
									//NFC_on(1);
									nfc_get_all_the_written_values(NFC_SLAVE_ADD,&tempThreshold, &humidThreshold);
									//NFC_on(0);
									LOG_INFO("Temperature Threshold is= %d, Humidity Threshold is = %d", tempThreshold,humidThreshold);
									systemState = SYSTEM_TRANSIT;
									prevState =	SYSTEM_DOC_SRC;
									dock_button_status = BUTTON_NONE;
								}
								else
								{
									if(printOnce == 1)
									{
										LOG_INFO("Change the switch to Dock Mode");
										printOnce = 0;
									}

								}
		break;
		/*The mode which is enabled when the package is in transit. Achieved by changing the dock-switch to transit
		position. All communication interfaces initialised. All sensors intialised.*/
		case(SYSTEM_TRANSIT):

								if(((prevState == SYSTEM_DOC_SRC)| (prevState == SYSTEM_TRANSIT)) && (dock_button_status == BUTTON_TRANSIT))
								{
									if(transit_initialisations == 1)
									{
										//LOG_INFO();
										LOG_INFO("********** STATE : TRANSIT, SWITCH : TRANSIT **********");
										transitMode_initialisations();
										LOG_INFO("BME 280 Initialised, Motion and Magnetic Detection Enabled");
										interrupt_enable();
										LOG_INFO("All interrupts enabled");
										transit_initialisations = 0;
									}
									task_scheduler();

									prevState = SYSTEM_TRANSIT;
									systemState = SYSTEM_TRANSIT;
									//SLEEP_Sleep();
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
		/*Final stage of the system. Interrupts disabled and only NFC i2c is enabled. Transfers the
		 * data stored during the transit to the NFC EEPROM from where the data can be read on the Android Application.*/
		case(SYSTEM_DOC_DESTN):
		LOG_INFO("********** STATE : DOCKING DESTINATION, SWITCH : DOCK **********");
		interrupt_disable();
		LOG_INFO("All interrupts disabled");
		dockingMode_initialisations();
		LOG_INFO("Docking Mode Initialisations Complete");
		nfc_i2c_write_data_to_nfc(NFC_SLAVE_ADD, sector);
		LOG_INFO("DATA WRITE TO APPLICATION VIA NFC COMPLETE");
		LOG_INFO();
		LOG_INFO();
		LOG_INFO("POWER CYCLE TO USE THE SYSTEM AGAIN ");
		prevState = SYSTEM_DOC_DESTN;
		systemState = SYSTEM_TRANSIT;
		//SLEEP_Sleep();
		break;

		}
	}//ending while


}//ending main



void task_scheduler()
{
	if((scheduler & TIMESTAMP_1SEC) == TIMESTAMP_1SEC)
	{
		timestamp_var = timestamp_var + 10;
		{
			return_status = get_temp_pres_humidity(tempThreshold, humidThreshold);
			if((return_status == EVENT_TEMP_HIGH) || (return_status == EVENT_HUMID_HIGH) || (return_status == EVENT_TEMP_HUMID_HIGH))
			{
				nfc_data_update(timestamp_var,return_status);
			}
			//second_count = 0;
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
				LOG_INFO("MOTION DETECTED");
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
			uint8_t source = i2c_write_read(0X53,1);
			{
				LOG_INFO("MAGNETIC FIELD DETECTED");
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

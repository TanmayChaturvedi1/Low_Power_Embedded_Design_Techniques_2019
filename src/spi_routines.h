/*
 * @filename 	: spi_routines.h
 * @description	: contains SPI initialization and data acquisition routines
 * @author		: Tanmay Chaturvedi, Puneet Bansal, and Nachiket Kelkar
 */


#ifndef SRC_SPI_ROUTINES_H_
#define SRC_SPI_ROUTINES_H_

#include "em_gpio.h"
#include "em_chip.h"
#include "em_device.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "bme280.h"
#include "bme280_defs.h"
#include "log.h"
#include "em_letimer.h"
#include "gpio.h"
#include "letimer.h"

#define	SIO			0
#define	INTERRUPT	1
#define	INTERRUPT_DRIVEN1

/* USART Configurations */
// USART1
#define BME280_USART                USART1
#define BME280_USART_CLK            cmuClock_USART1
#define BME280_USART_ROUTE          GPIO->USARTROUTE[1]
#define BME280_BAUDRATE				( 1000000 )	// 1MHz. Max supported frequency is 10 MHz.

/* BME280 SPI PIN Configurations */
#define BME280_SPI_MISO_PIN                     (13U)
#define BME280_SPI_MISO_PORT					(gpioPortD)

#define BME280_SPI_MOSI_PIN                     (12U)
#define BME280_SPI_MOSI_PORT					(gpioPortD)

#define BME280_SPI_SCLK_PIN                     (14U)
#define BME280_SPI_SCLK_PORT					(gpioPortD)

#define BME280_SPI_CS_PIN                     	(15U)
#define BME280_SPI_CS_PORT						(gpioPortD)

typedef enum {
	EVENT_MOTION_DETECTED,
	EVENT_MAGNETIC_FIELD_DETECTED,
	EVENT_TEMP_HIGH,
	EVENT_HUMID_HIGH,
	EVENT_TEMP_HUMID_HIGH
}
sensor_return_status;

uint32_t TxBufferIndex;
uint32_t RxBufferIndex;
bool timer_expired;
uint8_t temp_threshold;
uint8_t humid_threshold;

/* Basic functions for SPI - BME280*/
/***********************************************************************************************************
 * Function name: BME280_CS_High()                                                 	   					   *
 * Description: Function to pull the Chip Select Pin High and stop the SPI communication for BME280 sensor *
 * @param: void                                                                        					   *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_CS_High( void );

/***********************************************************************************************************
 * Function name: BME280_CS_Low()                                                 	   					   *
 * Description: Function to pull the Chip Select Pin Low and start the SPI communication for BME280 sensor *
 * @param: void                                                                        					   *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_CS_Low( void );

/***********************************************************************************************************
 * Function name: BME280_InsertDummyCycle()                                                 	   		   *
 * Description: Function to insert dummy cycles for the BME280 sensor									   *
 * @param: dummy_cycle number of dummy cycles                                                              *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_InsertDummyCycle( uint8_t dummy_cycle );

/***********************************************************************************************************
 * Function name: BME280_SendByte()                                                 	   				   *
 * Description: Function to send 1 byte via SPI to the BME280 sensor									   *
 * @param: byte_value to be written, transfer_type (Single Input Output)								   *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_SendByte( uint8_t byte_value, uint8_t transfer_type );

/***********************************************************************************************************
 * Function name: BME280_GetByte()                                                 	   				       *
 * Description: Function to receive 1 byte via SPI to the BME280 sensor									   *
 * @param: transfer_type (Single Input Output)															   *                                                                        					   *
 * @return: received data                                                                       		   *
 ***********************************************************************************************************/
uint8_t BME280_GetByte( uint8_t transfer_type );

/***********************************************************************************************************
 * Function name: BME280_SPI_init()                                                 	   				   *
 * Description: Function to initialize HFPER SPI Clock for the BME280 sensor							   *
 * @param: void								                                                               *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_SPI_init( void );

/***********************************************************************************************************
 * Function name: BME280_SPI_deinit()                                                 	   				   *
 * Description: Function to de-initialize HFPER SPI Clock for the BME280 sensor							   *
 * @param: void								                                                               *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_SPI_deinit( void );

/***********************************************************************************************************
 * Function name: BME280_user_spi_write()                                                 	   			   *
 * Description: Function API to error-check and transmit number of bytes to BME280 sensor				   *
 * @param: sensor ID, reg_address to be written to, data_buffer to store returned data, len of data to be written  *
 * @return: status of transfer                                                               					   *
 ***********************************************************************************************************/
int8_t BME280_user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );

/***********************************************************************************************************
 * Function name: BME280_user_spi_read()                                                 	   			   *
 * Description: Function API to error-check and receive number of bytes from the BME280 sensor			   *
 * @param: sensor ID, reg_address to be written to, data_buffer to store returned data, len of data to be read *
 * @return: status of transfer                                                                       	   *
 ***********************************************************************************************************/
int8_t BME280_user_spi_read( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );

/***********************************************************************************************************
 * Function name: BME280_user_delay_ms()                                                 	   			   *
 * Description: Function to provide delay in mili seconds 												   *
 * @param: delay in miliseconds																			   *								                                                               *          					   *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void BME280_user_delay_ms( uint32_t period );

/***********************************************************************************************************
 * Function name: BME280_Device_init()                                                 	   				   *
 * Description: Function to initialize the BME280 sensor							   					   *
 * @param: void								                                                               *
 * @return: result of SPI success    									             					   *
 ***********************************************************************************************************/
int8_t BME280_Device_init( void );

/***********************************************************************************************************
 * Function name: BME280_SPI_deinit()                                                 	   				   *
 * Description: Function API that invokes bme280_get_sensor_data() and pass along trimming and calibration data for the sensor *
 * @param: pointer to struct bme280_dev					                                                   *
 * @return: result of SPI transaction               	                            					   *
 ***********************************************************************************************************/
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev);

/***********************************************************************************************************
 * Function name: print_sensor_data()                                                 	   				   *
 * Description: Function to print temperature, pressure and humidity data    							   *
 * @param: pointer to struct bme280_data	                                                               *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void print_sensor_data(struct bme280_data *comp_data);

/***********************************************************************************************************
 * Function name: get_temp_pres_humidity()                                                 	 			   *
 * Description: Function API to return sensor event information from the BME280 sensor  				   *
 * @param: void								                                                    	       *
 * @return: enum sensor_return_status                                                               	   *
 ***********************************************************************************************************/
sensor_return_status get_temp_pres_humidity(int32_t, int32_t);

/***********************************************************************************************************
 * Function name: BME280_SPI_deinit()                                                 	   				   *
 * Description: Function to get the inbuilt calibration and trimming information stored in the BME280 sensor *
 * @param: void								                                                               *
 * @return: void                                                                       					   *
 ***********************************************************************************************************/
void temp_sensor_init();

#endif /* SRC_SPI_ROUTINES_H_ */

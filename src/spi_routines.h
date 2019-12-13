/*
 * spi_routines.h
 *
 *  Created on: Nov 26, 2019
 *      Author: TanmayC
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
	EVENT_TEMP_HUMID_NORMAL,
	EVENT_TEMP_HIGH,
	EVENT_HUMID_HIGH,
	EVENT_TEMP_HUMID_HIGH
}
temp_humidity_return_status_e;

/* Temp sensor defines*/

//uint16_t TxBuffer[1] = {0x0F};
//uint16_t RxBuffer[1];

/*struct bme280_dev dev;
struct bme280_data *comp_data;*/

uint32_t TxBufferIndex;
uint32_t RxBufferIndex;
bool timer_expired;


/* Basic functions */
void BME280_CS_High( void );
void BME280_CS_Low( void );
void BME280_InsertDummyCycle( uint8_t dummy_cycle );
void BME280_SendByte( uint8_t byte_value, uint8_t transfer_type );
uint8_t BME280_GetByte( uint8_t transfer_type );
void BME280_SPI_init( void );
void BME280_SPI_deinit( void );
int8_t BME280_user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
int8_t BME280_user_spi_read( uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len );
void BME280_user_delay_ms( uint32_t period );
int8_t BME280_Device_init( void );
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev);
void print_sensor_data(struct bme280_data *comp_data);
temp_humidity_return_status_e get_temp_pres_humidity(int32_t temp_threshold, int32_t humid_threshold);

#endif /* SRC_SPI_ROUTINES_H_ */

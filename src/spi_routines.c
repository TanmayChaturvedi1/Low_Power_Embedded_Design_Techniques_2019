/*
 * spi_routines.c
 *
 *  Created on: Nov 26, 2019
 *      Author: TanmayC
 *
 *  BME280 Sensor data parsing APIs referred from:
 *  www.github.com/BoschSensortec/BME280_driver
 */

#include "spi_routines.h"
struct bme280_dev dev;
struct bme280_data comp_data;
bool data_received;


void BME280_SPI_init( void )
{
   LOG_INFO("SPI init invoked\n");

   CMU_ClockEnable(cmuClock_HFPER, true);
   CMU_ClockEnable( cmuClock_GPIO, true );
   CMU_ClockEnable( BME280_USART_CLK, true );

   /* IO config */
   GPIO_PinModeSet( BME280_SPI_MOSI_PORT, BME280_SPI_MOSI_PIN, gpioModePushPull, 1 );
   GPIO_PinModeSet( BME280_SPI_MISO_PORT, BME280_SPI_MISO_PIN, gpioModeInput,    1 );
   GPIO_PinModeSet( BME280_SPI_SCLK_PORT, BME280_SPI_SCLK_PIN, gpioModePushPull, 0 );
   GPIO_PinModeSet( BME280_SPI_CS_PORT,   BME280_SPI_CS_PIN,   gpioModePushPull, 0 );


   USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;

   config.master       = true;            // master mode
   config.baudrate     = 1000000;         // CLK freq is 1 MHz
  // config.autoCsEnable = true;            // CS pin controlled by hardware, not firmware
   config.clockMode    = usartClockMode0; // clock idle low, sample on rising/first edge
   config.msbf         = true;            // send MSB first
   config.enable       = usartDisable;    // Make sure to keep USART disabled until it's all set up
   USART_InitSync(USART1, &config);

   USART_InitSync( BME280_USART, &config );

   // Set USART pin locations
   USART1->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC20) | // US1_CLK       on location 20 = PD14 per datasheet section 6.4 = EXP Header pin 8
                       (USART_ROUTELOC0_TXLOC_LOC20)  | // US1_TX (MOSI) on location 20 = PC6 per datasheet section 6.4 = EXP Header pin 4
                       (USART_ROUTELOC0_RXLOC_LOC20);   // US1_RX (MISO) on location 20 = PC7 per datasheet section 6.4 = EXP Header pin 6

   // Enable USART pins
   USART1->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

#ifdef INTERRUPT_DRIVEN
   // Enabling TX interrupts to transfer whenever
   // there is room in the transmit buffer
   USART_IntClear(USART1, USART_IF_TXBL);
   USART_IntEnable(USART1, USART_IF_TXBL);
   NVIC_ClearPendingIRQ(USART1_TX_IRQn);
   NVIC_EnableIRQ(USART1_TX_IRQn);

   // Enabling RX interrupts to trigger whenever
   // a new packet arrives from the slave
   USART_IntClear(USART1, USART_IF_RXDATAV);
   USART_IntEnable(USART1, USART_IF_RXDATAV);
   NVIC_ClearPendingIRQ(USART1_RX_IRQn);
   NVIC_EnableIRQ(USART1_RX_IRQn);
#endif

   // Enable USART1
   USART_Enable(USART1, usartEnable);

}

int8_t BME280_Device_init( void )
{
	int8_t result = BME280_OK;

	/* Sensor_0 interface over SPI with native chip select line */
	dev.dev_id = 0;
	dev.intf = BME280_SPI_INTF;
	dev.read = BME280_user_spi_read;
	dev.write = BME280_user_spi_write;
	dev.delay_ms = BME280_user_delay_ms;

	result = bme280_init(&dev);
	LOG_INFO("result = %d\n", result);
	return result;
}

void BME280_SPI_deinit( void )
{
  USART_Enable( BME280_USART, false );
  GPIO_PinModeSet( BME280_SPI_MOSI_PORT, BME280_SPI_MOSI_PIN, gpioModeDisabled, 0);
  GPIO_PinModeSet( BME280_SPI_MISO_PORT, BME280_SPI_MISO_PIN, gpioModeDisabled, 0);
  GPIO_PinModeSet( BME280_SPI_SCLK_PORT, BME280_SPI_SCLK_PIN, gpioModeDisabled, 1);
  GPIO_PinModeSet( BME280_SPI_CS_PORT,   BME280_SPI_CS_PIN,   gpioModeDisabled, 1);
}

/*
 * Function:       CS_Low, CS_High
 * Arguments:      None.
 * Description:    Chip select go low / high.
 * Return Message: None.
 */
void BME280_CS_Low()
{
   GPIO_PinOutClear( BME280_SPI_CS_PORT, BME280_SPI_CS_PIN );
}

void BME280_CS_High()
{
   GPIO_PinOutSet( BME280_SPI_CS_PORT, BME280_SPI_CS_PIN );
}

/*
 * Function:       InsertDummyCycle
 * Arguments:      dummy_cycle, number of dummy clock cycle
 * Description:    Insert dummy cycle of SCLK
 * Return Message: None.
 */
void BME280_InsertDummyCycle( uint8_t dummy_cycle )
{
   int i;

   for( i = 0; i < dummy_cycle/8; i++ )
   {
      USART_SpiTransfer( BME280_USART, 0xff );
   }
}

/*
 * Function:       SendByte
 * Arguments:      byte_value, data transfer to flash
 *                 transfer_type, select different type of I/O mode.
 *                 Seven mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 PIO, parallel
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 *                 INTERRUPT, Uses USART_Tx() and USART_Rx()
 * Description:    Send one byte data to flash
 * Return Message: None.
 */
void BME280_SendByte( uint8_t byte_value, uint8_t transfer_type )
{
   switch( transfer_type )
   {
   case SIO: // Single I/O, polled
      USART_SpiTransfer( BME280_USART, byte_value );
      break;
   case INTERRUPT:
	  USART_Tx( BME280_USART, byte_value );
	  break;
   default:
      break;
   }
}

/*
 * Function:       GetByte
 * Arguments:      byte_value, data receive from flash
 *                 transfer_type, select different type of I/O mode.
 *                 Seven mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 PIO, parallel IO
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 * Description:    Get one byte data to flash
 * Return Message: 8 bit data
 */
uint8_t BME280_GetByte( uint8_t transfer_type )
{
   uint8_t data_buf = 0;

   switch( transfer_type )
   {
   case SIO: // Single I/O
      //--- insert your code here for single IO receive. ---//
      data_buf = USART_SpiTransfer( BME280_USART, 0xFF );
      break;
   case INTERRUPT:
	  data_buf = USART_Rx( BME280_USART );
	  break;
   default:
      break;
   }

    return data_buf;
}


int8_t BME280_user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus.
     * Ignoring check for dev_id as only one device used currently
     */

    // Chip select go low to start a BME280 command
    BME280_CS_Low();

#ifdef INTERRUPT_DRIVEN
    BME280_SendByte( reg_addr, INTERRUPT );
        // Send command
    if( data_received )
    {
        for( int i = 0; i < len; i++)
        {
        	*(reg_data + i) = BME280_GetByte( INTERRUPT );
        }
    }
#else
    BME280_SendByte( reg_addr, SIO );
        // Send command
        for( int i = 0; i < len; i++)
        {
        	*(reg_data + i) = BME280_GetByte( SIO );
        }
#endif

    // Chip select go high to end a BME280 command
    BME280_CS_High();

    /*
     * Data on the bus should be like
     * |----------------+---------------------+-------------|
     * | MOSI           | MISO                | Chip Select |
     * |----------------+---------------------|-------------|
     * | (don't care)   | (don't care)        | HIGH        |
     * | (reg_addr)     | (don't care)        | LOW         |
     * | (don't care)   | (reg_data[0])       | LOW         |
     * | (....)         | (....)              | LOW         |
     * | (don't care)   | (reg_data[len - 1]) | LOW         |
     * | (don't care)   | (don't care)        | HIGH        |
     * |----------------+---------------------|-------------|
     */

    return rslt;
}

int8_t BME280_user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     * Ignoring check for dev_id as only one device used currently
     */

    //Chip select go low to start a BME280 command
	BME280_CS_Low();
    // Send command
#ifdef	INTERRUPT_DRIVEN
    for( int i = 0; i < len; i++)
    {
    	BME280_SendByte( reg_addr, INTERRUPT );
    	BME280_SendByte( *(reg_data+i), INTERRUPT );
    }
#else
    for( int i = 0; i < len; i++)
        {
        	BME280_SendByte( reg_addr, SIO );
        	BME280_SendByte( *(reg_data+i), SIO );
        }
#endif
    // Chip select go high to end a BME280 command
    BME280_CS_High();
    /*
     * Data on the bus should be like
     * |---------------------+--------------+-------------|
     * | MOSI                | MISO         | Chip Select |
     * |---------------------+--------------|-------------|
     * | (don't care)        | (don't care) | HIGH        |
     * | (reg_addr)          | (don't care) | LOW         |
     * | (reg_data[0])       | (don't care) | LOW         |
     * | (....)              | (....)       | LOW         |
     * | (reg_data[len - 1]) | (don't care) | LOW         |
     * | (don't care)        | (don't care) | HIGH        |
     * |---------------------+--------------|-------------|
     */

    return rslt;
}

void BME280_user_delay_ms(uint32_t period)
{
    /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
	uint32_t initial_ticks , max_ticks, count_till, us_ticks;
	uint32_t freq = 1000; //for ULFRCXO
	period = period * 1000; //period is in miliseconds and below calculations assumes microsecond
	us_ticks = freq * period;
	us_ticks = us_ticks/1000000;
		initial_ticks = LETIMER_CounterGet( LETIMER0 );
		if(initial_ticks > us_ticks)
		{
			count_till = initial_ticks - us_ticks;
			while( LETIMER_CounterGet( LETIMER0 ) != count_till );
		}
		else
		{
			max_ticks = LETIMER_CompareGet( LETIMER0, 0 );
			while(LETIMER_CounterGet( LETIMER0 ) != (max_ticks - (us_ticks - initial_ticks)));
		}
}

int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
	int8_t rslt;
	uint8_t settings_sel;
	//struct bme280_data comp_data;

	/* Recommended mode of operation: Indoor navigation */
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

	LOG_INFO("Temperature, Pressure, Humidity\r\n");
		/* Delay while the sensor completes a measurement */
		dev->delay_ms(70);
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
		print_sensor_data(&comp_data);

	return rslt;
}

void print_sensor_data(struct bme280_data *comp_data)
{
#ifdef BME280_FLOAT_ENABLE
        LOG_INFO("%0.2f, %0.2f, %0.2f\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
#else
        LOG_INFO("%ld, %ld, %ld\r\n",comp_data->temperature, comp_data->pressure, comp_data->humidity);
#endif
}


void USART1_TX_IRQHandler(void)
{
	data_received = TRUE;
	LOG_INFO("In TX Interrupt\n");
  // Stop sending once we've gone through the whole TxBuffer
}

/**************************************************************************//**
 * @brief USART1 RX IRQ Handler
 *****************************************************************************/
void USART1_RX_IRQHandler(void)
{
	LOG_INFO("In RX Interrupt\n");
}


// API to get sensor event info based on temperature and humidity threshold
temp_humidity_return_status_e get_temp_pres_humidity(int32_t temp_threshold, int32_t humid_threshold)
{
	int32_t temperature;
	uint32_t pressure;
	uint32_t humidity;
	temp_threshold = temp_threshold * 100;	//temperature is returned with units 100 * deg Celcius
	humid_threshold = humid_threshold*(1024);	//humidity is returned with units 1024 * %relative_humidity
	int8_t rslt;
	rslt = stream_sensor_data_normal_mode(&dev);
	temperature = comp_data.temperature;
	printf("received temp_data = %ld",comp_data.temperature);
	pressure = comp_data.pressure;
	printf("received pressure_data = %ld",comp_data.pressure);
	humidity = comp_data.humidity;
	printf("received humid_data = %ld",comp_data.humidity);

	if ( temperature >= temp_threshold && humidity >= humid_threshold )
		return EVENT_TEMP_HUMID_HIGH;
	else if ( temperature >= temp_threshold )
		return EVENT_TEMP_HIGH;
	else if ( humidity >= humid_threshold )
		return EVENT_HUMID_HIGH;
	else
		return EVENT_TEMP_HUMID_NORMAL;
}



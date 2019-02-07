/*
 * @filename	: si7021.c
 * @description	: This file contains functions to obtain temperature value from si7021
 * @author 		: Puneet Bansal
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 * 				  si7021 datasheet -https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 */
#include "si7021.h"
#include "main.h"

/*
 * @description
 * Populate the I2C_TransferSeq structure with slave address, the type of transfer (No hold master mode).
 * Call I2c write function, wait for atlest 3.8 ms before reading the value from slave using i2c_read();
 * Calculated the temperature in degree celcius from the ADC value received from sensor.
 *
 */
void temp_get()
{
	float tempDeg;
	uint16_t tempvalue;
	I2C_TransferSeq_TypeDef init;

	/*Coommon configurations for both i2c read and write*/
	init.addr= SLAVE_ADD << 1;
	uint8_t command=NO_HOLD_MASTER_TEMP;	/*Get temperature, use hold master mode*/
	init.buf[0].data=&command;

	/*Write operation to transfer slave address and type of data required*/
	i2c_write(init);
	timerWaitUs(10000);
	/*Read operation to read temperature from Si7021*/

	tempvalue=i2c_read(init);					 /*Perform the read operation*/
	tempDeg= (((tempvalue*175.72)/65536)-46.85); /*Convert the temperature value in degree*/
	LOG_INFO("Temp %f",tempDeg);
}

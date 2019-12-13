/*
 * @filename	: i2c.c
 * @description	: This file contains functions to configure I2C peripheral.
 * @author 		: Puneet Bansal
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 *
 */
#include "i2c.h"
#include "main.h"



/*
 * @decription
 * initialise i2c module with default settings
 * Set the approriate SCL and SDA port ,pin and alternate function number
 *
 */
void i2c_init()
{
	I2CSPM_Init_TypeDef init1 = I2CSPM_INIT_DEFAULT;

	#if GEEKY_PANDA_I2C
		init1.sclPin=SCL_PIN;
		init1.sclPort=SCL_PORT;
		init1.sdaPin=SDA_PIN;
		init1.sdaPort=SDA_PORT;
		init1.portLocationScl=30;
		init1.portLocationSda=30;
	#endif

	I2CSPM_Init(&init1);
}

/*
 * @description
 * function to perform write to slave.
 * writing the slave address, mode to hold master mode, length to 1 byte.
 * if the transfer is not complete then logged the error.
 *
 * @param: init
 * instance of TransferSeq_TypeDef passed from temp_get() in si7021.c
 */

void i2c_write(I2C_TransferSeq_TypeDef init)
{
	I2C_TransferReturn_TypeDef ret;

	init.buf[0].len=1;
	init.flags= I2C_FLAG_WRITE;


	ret=I2CSPM_Transfer(I2C0,&init);
	if(ret != i2cTransferDone)
	{
		LOG_ERROR("I2C Write error");
		return;
	}
}

/*
 * @description:
 * function to read temperature from slave. Set received_data to point to buf[0].data buffer.
 * calculated the 12bit temperature value from the received_data.
 * received_data[0] consists MSB, received_data[1] consists LSB
 *
 * @param: init
 * instance of TransferSeq_TypeDef passed from temp_get() in si7021.c
 *
 */
uint16_t i2c_read(I2C_TransferSeq_TypeDef init)
{
	I2C_TransferReturn_TypeDef ret;
	uint8_t received_data[2] = {0};			/*Buffer to receive the data recieved from slave*/
	uint16_t tempvalue;						/*Storing the ADC value given by sensor*/
	init.flags= I2C_FLAG_READ;

	init.buf[0].data= received_data;
	init.buf[0].len=sizeof(received_data);//received_data=*(init.buf[0].data);
	ret=I2CSPM_Transfer(I2C0,&init);

	if(ret != i2cTransferDone)
	{
		LOG_ERROR("I2C Read error");
	}

	tempvalue = received_data[0];
	tempvalue <<= 8;
	tempvalue	|= (received_data[1]);
	LOG_INFO("bufer[0] is %x",received_data[0]);
	LOG_INFO("bufer[1] is %x",received_data[1]);
	return tempvalue;
}
I2C_TransferSeq_TypeDef write_read_init;
uint8_t received_data[1] = {0};
uint16_t tempvalue;

uint16_t i2c_write_read(uint8_t regToRead, uint8_t len)
{
	I2C_TransferReturn_TypeDef ret;

	write_read_init.buf[0].len=1;
	write_read_init.addr= ACC_SLAVE_ADD << 1;
	//uint8_t command=WHO_AM_I_ACC;	/*Get temperature, use hold master mode*/
	write_read_init.buf[0].data= &regToRead;


	write_read_init.buf[1].data= received_data;
	write_read_init.buf[1].len=len;//sizeof(received_data);
	write_read_init.flags= I2C_FLAG_WRITE_READ;

	//received_data=*(init.buf[0].data);
	ret=I2CSPM_Transfer(I2C0,&write_read_init);
	if(ret != i2cTransferDone)
		{
			LOG_ERROR("I2C Error");
			//GPIO_PinOutSet(gpioPortD,10);
			//GPIO_PinOutSet(gpioPortA,1);
			//GPIO_PinOutSet(gpioPortD,10);
		}

	//LOG_INFO("bufer[0] is %x",received_data[0]);
	//LOG_INFO("bufer[1] is %x",received_data[1]);
	//LOG_INFO("value is %x",((int16_t)received_data[0]<<8) | (received_data[1]));
	return received_data[0];


}

I2C_TransferSeq_TypeDef write_write_init;


void i2c_write_write(uint16_t regName, uint16_t data)
{
	I2C_TransferReturn_TypeDef ret;

	write_write_init.addr= ACC_SLAVE_ADD << 1;
	write_write_init.buf[0].len=1;
	write_write_init.buf[0].data = &regName;

	write_write_init.buf[1].len=1;
	write_write_init.buf[1].data = &data;
	write_write_init.flags= I2C_FLAG_WRITE_WRITE;

	ret=I2CSPM_Transfer(I2C0,&write_write_init);

	if(ret == i2cTransferNack)
	{
		LOG_ERROR("I2C Write error");
		//GPIO_PinOutSet(gpioPortD,10);

		return;
	}
}

I2C_TransferSeq_TypeDef write_init;

uint8_t dataBuffer[]={0};

void i2c_write_new(uint8_t writeRegister , uint8_t val)
{
	I2C_TransferReturn_TypeDef ret;

	write_init.addr= ACC_SLAVE_ADD << 1;
	dataBuffer[0] = writeRegister;
	dataBuffer[1]= val;
	write_read_init.buf[0].data= dataBuffer;
	write_init.buf[0].len=2;
	write_init.flags= I2C_FLAG_WRITE;


	ret=I2CSPM_Transfer(I2C0,&write_init);
	if(ret != i2cTransferDone)
	{
		LOG_ERROR("I2C Write error");
		//GPIO_PinOutSet(gpioPortD,10);
		return;
	}


	//uint8_t command=WHO_AM_I_ACC;	/*Get temperature, use hold master mode*/

}

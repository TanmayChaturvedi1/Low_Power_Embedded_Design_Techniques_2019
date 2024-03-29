/*
 * @filename	: i2c.c
 * @description	: This file contains functions to configure I2C peripheral.
 * @author 		: Puneet Bansal, Nachiket Kelkar, Tanmay Chaturvedi.
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 *                Assignments developed for the course IoT Embedded Firmware.
 */
#include "i2c.h"
#include "main.h"

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

	ret=I2CSPM_Transfer(I2C0,&write_read_init);
	if(ret != i2cTransferDone)
		{
			LOG_ERROR("I2C Error");
		}
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
		return;
	}

}

uint8_t i2c_data[16]={5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
uint16_t i2c_length;
I2C_TransferSeq_TypeDef transfer_seq;
I2C_TransferReturn_TypeDef status;
extern uint8_t nfc_data[117][16];

void nfc_i2c_init()
{
	I2CSPM_Init_TypeDef i2c_config = I2CSPM_INIT_DEFAULT;
	i2c_config.port = I2C0;
	i2c_config.sclPort = gpioPortC;
	i2c_config.sclPin = 11;
	i2c_config.sdaPort = gpioPortC;
	i2c_config.sdaPin = 10;
	i2c_config.portLocationScl = 15;
	i2c_config.portLocationSda = 15;
	I2CSPM_Init(&i2c_config);
}

uint8_t* nfc_i2c_read_data_spm(uint8_t addr, uint8_t sector)
{
	// Writing sector to the NFC to point to it.
	transfer_seq.addr		  = addr << 1;
	transfer_seq.buf[0].data  = &sector;
	transfer_seq.buf[0].len   = 1;
	transfer_seq.flags		  = I2C_FLAG_WRITE;

	status = I2CSPM_Transfer(I2C0,&transfer_seq);
	for(volatile int i=0;i<10000;i++);

	// Read the sector data from the NFC module.
	transfer_seq.addr		  = addr << 1;
	transfer_seq.buf[0].data  = i2c_data;
	transfer_seq.buf[0].len   = 16;
	transfer_seq.flags		  = I2C_FLAG_READ;

	status = I2CSPM_Transfer(I2C0,&transfer_seq);
	for(volatile int i=0;i<60000;i++);
	return i2c_data;
}

void nfc_i2c_write_data_spm(uint8_t address, uint8_t sector, uint8_t* write_data)
{
	for(int i=0;i<16;i++)
	{
		printf("%c ",write_data[i]);
	}
	printf("\n");

	transfer_seq.addr		  = address << 1;
	transfer_seq.buf[0].data  = &sector;
	transfer_seq.buf[0].len   = 1;
	transfer_seq.buf[1].data  = write_data;
	transfer_seq.buf[1].len   = 16;
	transfer_seq.flags		  = I2C_FLAG_WRITE_WRITE;

	status = I2CSPM_Transfer(I2C0,&transfer_seq);
	LOG_INFO("Status is %d",status);
}


void nfc_i2c_write_data_to_nfc(uint8_t address, uint8_t max_sectors)
{
	if (max_sectors < 55){
		for(int i=0; i<max_sectors; i++){
			nfc_i2c_write_data_spm(address, i+1, nfc_data[i]);
		}
	}
	else if(max_sectors > 54 && max_sectors <=127){
		for(int i=0; i<55; i++){
			nfc_i2c_write_data_spm(address, i+1, nfc_data[i]);
		}
		for(int i=64; i<=max_sectors+9; i++){
			nfc_i2c_write_data_spm(address, i, nfc_data[i-9]);
		}
	}
}



void nfc_get_all_the_written_values(uint8_t address, uint8_t* temperature_th, uint8_t* humidity_th)
{
	uint8_t* data;
	uint8_t temp1, temp2;

	// Get the sectors 0x01 data and update the timestamp
	data = nfc_i2c_read_data_spm(address, 0x01);

	nfc_data[1][0] = data[9];
	nfc_data[1][1] = data[10];
	nfc_data[1][2] = data[11];
	nfc_data[1][3] = data[12];
	nfc_data[1][4] = data[13];
	nfc_data[1][5] = data[14];
	nfc_data[1][6] = data[15];


	data = nfc_i2c_read_data_spm(address, 0x02);
	nfc_data[1][7] = data[0];
	nfc_data[1][8] = data[1];
	nfc_data[1][9] = data[2];
	nfc_data[1][10] = data[3];
	nfc_data[1][11] = data[4];
	nfc_data[1][12] = data[5];

	for(int i=0; i<13; i++)
	{
		printf("%c ",nfc_data[1][i]);
	}

	// Get the temperature threshold from the sector
	temp1 = data[6] - 48;					// Update the actual location of the higher threshold value character
	temp2 = data[7] - 48;					// Update the actual location of the lower threshold value character
	*temperature_th = ((temp1 * 10) + temp2);


	// Get the humidity threshold from the sector
	temp1 = data[8] - 48;					// Update the actual location of the higher threshold value character
	temp2 = data[9] - 48;					// Update the actual location of the lower threshold value character
	*humidity_th    = ((temp1 * 10) + temp2);
}


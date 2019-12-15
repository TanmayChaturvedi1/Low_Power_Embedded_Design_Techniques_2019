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

void motion_detection()
{
	//Set the device in 50 Hz ODR, Standby
	i2c_write_new(0x2A, 0X08); //0X18
	//i2c_write_new(0x2A,0X18);
	timerWaitUs(10000);
	//LOG_INFO("%x", i2c_write_read(0X2A, 1));
	timerWaitUs(10000);

	//Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling X,
	//Y, Z and the Latch
	i2c_write_write(0x15, 0XD8);
	//i2c_write_new(0x15,0XD8);
	timerWaitUs(1000);
	//LOG_INFO("%x", i2c_write_read(0X15, 1));


	//setting threshold for resulting acceleration <0.2g
	i2c_write_write(0x17, 0X30);
	//i2c_write_new(0x17,0X30);
	timerWaitUs(1000);
	//LOG_INFO("%x", i2c_write_read(0X17, 1));


	//setting the debounce counter to 120ms
	i2c_write_write(0x18, 0X0A);
	//i2c_write_new(0x18,0X0A);
	timerWaitUs(1000);
	//LOG_INFO("%x", i2c_write_read(0X18, 1));


	//Enabling motion/freefall interrupt in the system (ctrl_reg4)
	i2c_write_write(0x2D, 0X04);
	//i2c_write_new(0x2D,0X04);
	timerWaitUs(1000);
	//LOG_INFO("%x", i2c_write_read(0X2D, 1));


	//Route the interrupt function to INT2 hardware pin (ctrl_reg4)
	i2c_write_write(0x2E, 0X04);
	//i2c_write_new(0x2E,0X04);
	timerWaitUs(1000);
	//LOG_INFO("%x", i2c_write_read(0X2E, 1));


	//putting the device in active mode
	uint16_t val = i2c_write_read(0X2A, 1);
	timerWaitUs(1000);
	i2c_write_write(0x2A, (uint8_t)val|0x01);
	//i2c_write_new(0x2A,(uint8_t)val|0x01);
	timerWaitUs(1000);
	//LOG_INFO("val is %x", (uint8_t)val|0x01);
	//LOG_INFO("%x", i2c_write_read(0X2A, 1));

}

void freefall_detection()
{
	//Set the device in 50 Hz ODR, Standby
	i2c_write_write(0x2A, 0X20);
	timerWaitUs(10000);
	LOG_INFO("%x", i2c_write_read(0X2A, 1));
	timerWaitUs(80000);

	//Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling X,
	//Y, Z and the Latch
	i2c_write_write(0x15, 0XB8);
	timerWaitUs(80000);
	LOG_INFO("%x", i2c_write_read(0X15, 1));


	//setting threshold for resulting acceleration <0.2g
	i2c_write_write(0x17, 0X03);
	timerWaitUs(80000);
	LOG_INFO("%x", i2c_write_read(0X17, 1));


	//setting the debounce counter to 120ms
	i2c_write_write(0x18, 0X06);
	timerWaitUs(80000);
	LOG_INFO("%x", i2c_write_read(0X18, 1));


	//Enabling motion/freefall interrupt in the system (ctrl_reg4)
	i2c_write_write(0x2D, 0X04);
	timerWaitUs(80000);
	LOG_INFO("%x", i2c_write_read(0X2D, 1));


	//Route the interrupt function to INT2 hardware pin (ctrl_reg4)
	i2c_write_write(0x2E, 0X00);
	timerWaitUs(80000);
	LOG_INFO("%x", i2c_write_read(0X2E, 1));


	//putting the device in active mode
	uint16_t val = i2c_write_read(0X2A, 1);
	i2c_write_write(0x2A, (uint8_t)val|0x01);
	timerWaitUs(80000);
	//LOG_INFO("val is %x", (uint8_t)val|0x01);
	LOG_INFO("%x", i2c_write_read(0X2A, 1));

}

void magnetic_magnitude_detection()
{
	//NVIC_DisableIRQ(GPIO_ODD_IRQn);
	//Soft Reset
	LOG_INFO("0");
	i2c_write_new(0x2B, 0X40);
	timerWaitUs(10000);
	LOG_INFO("%x", i2c_write_read(0X2B, 1));


	//i2c_write_write(0X2B, 0X40);
	//timerWaitUs(1000);
	LOG_INFO("1");

	//Initialize reference vector
	refMagX = refMagXHi = refMagXLo = 0;
	refMagY = refMagYHi = refMagYLo = 0;
	refMagZ = refMagZHi = refMagZLo = 0;

	//Initialize min/max for hard iron estimate
	magZMax = (int16_t)0x8000;
	magXMax = magZMax;
	magYMax = magZMax;
	magXMin = magYMin = magZMin = (int16_t)0x7FFF;

	//Set threshold. 1000 counts = 100.0uT
	magThreshold = 1000 ;//counts
	magThresholdHi = (magThreshold & 0xFF00) >> 8;
	magThresholdLo = magThreshold & 0xFF;

	//GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true,true );
	//NVIC_EnableIRQ(GPIO_ODD_IRQn);
	i2c_write_new(0x6A, (0x80 | magThresholdHi));
	timerWaitUs(1000);
	LOG_INFO("%x", i2c_write_read(0X6A, 1));
	LOG_INFO("2");


	i2c_write_write(0x6B, magThresholdLo);
	timerWaitUs(1000);
	LOG_INFO("%x", i2c_write_read(0X6B, 1));
	LOG_INFO("3");

	//M_VECM_CNT = 1 * 20ms = 20ms
	//steps double in hybrid mode

	i2c_write_write(0x6C, 0x01);
	timerWaitUs(1000);
	LOG_INFO("3");

	i2c_write_write(0x6D, refMagXHi);
	timerWaitUs(1000);
	LOG_INFO("4");

	i2c_write_write(0x6E, refMagXLo);
	timerWaitUs(1000);
	LOG_INFO("5");

	i2c_write_write(0x6F, refMagYHi);
	timerWaitUs(1000);
	LOG_INFO("6");

	i2c_write_write(0x70, refMagYLo);
	timerWaitUs(1000);
	LOG_INFO("7");

	i2c_write_write(0x71, refMagZHi);
	timerWaitUs(1000);
	LOG_INFO("8");

	i2c_write_write(0x72, refMagZLo);
	timerWaitUs(1000);
	LOG_INFO("8");

	//M_VECM_CFG
	//m_vecm_ele = 1 => event latching enabled
	//m_vecm_initm = 1 => use M_VECM_INITX/Y/Z as initial reference
	//m_vecm_updm = 1 => do not update initial reference
	//m_vecm_en = 1 => enable magnetometer vector magnitude detection feature

	i2c_write_write(0x69, 0x7B);
	timerWaitUs(1000);
	LOG_INFO("9");

	i2c_write_write(0x2D, 0x01);
	timerWaitUs(1000);
	LOG_INFO("10");

	i2c_write_write(0x2E, 0x01);
	timerWaitUs(1000);
	LOG_INFO("11");

	//Setup device for hybrid mode, enable hybrid mode auto-increment,
	//ODR = 50Hz, OSR=32, go to ACTIVE mode using M_CTRL_REG1, M_CTRL_REG2 and
	//CTRL_REG1
	i2c_write_write(0x5B, 0x1F);
	timerWaitUs(1000);
	LOG_INFO("12");

	i2c_write_write(0x5C, 0x20);
	timerWaitUs(1000);
	LOG_INFO("13");

	i2c_write_write(0x2A, 0x19);
	timerWaitUs(1000);

	LOG_INFO("%x", i2c_write_read(0X2A, 1));
	LOG_INFO("14");

}

void auto_calliberation()
{
		//NVIC_DisableIRQ(GPIO_ODD_IRQn);
		//Soft Reset
		LOG_INFO("0");
		i2c_write_new(0x2B, 0X40);
		timerWaitUs(10000);
		LOG_INFO("%x", i2c_write_read(0X2B, 1));


		//i2c_write_write(0X2B, 0X40);
		//timerWaitUs(1000);
		LOG_INFO("1");

		//Initialize reference vector
		refMagX = refMagXHi = refMagXLo = 0;
		refMagY = refMagYHi = refMagYLo = 0;
		refMagZ = refMagZHi = refMagZLo = 0;

		//Initialize min/max for hard iron estimate
		magZMax = (int16_t)0x8000;
		magXMax = magZMax;
		magYMax = magZMax;
		magXMin = magYMin = magZMin = (int16_t)0x7FFF;

		//Set threshold. 1000 counts = 100.0uT
		magThreshold = 1000 ;//counts
		magThresholdHi = (magThreshold & 0xFF00) >> 8;
		magThresholdLo = magThreshold & 0xFF;

		//GPIO_ExtIntConfig(MOTION_DETECTION_PORT,MOTION_DETECTION_PIN, MOTION_DETECTION_PIN , false, true,true );
		//NVIC_EnableIRQ(GPIO_ODD_IRQn);
		i2c_write_new(0x6A, (0x80 | magThresholdHi));
		timerWaitUs(1000);
		LOG_INFO("%x", i2c_write_read(0X6A, 1));
		LOG_INFO("2");


		i2c_write_write(0x6B, magThresholdLo);
		timerWaitUs(1000);
		LOG_INFO("%x", i2c_write_read(0X6B, 1));
		LOG_INFO("3");

		//M_VECM_CNT = 1 * 20ms = 20ms
		//steps double in hybrid mode

		i2c_write_write(0x6C, 0x01);
		timerWaitUs(1000);
		LOG_INFO("3");

		i2c_write_write(0x69, 0x7B);
		timerWaitUs(1000);

		i2c_write_write(0x2D, 0x01);
				timerWaitUs(1000);

		i2c_write_write(0x2E, 0x01);
		timerWaitUs(1000);

		i2c_write_write(0x5B, 0x9F);
		timerWaitUs(1000);

		i2c_write_write(0x5C, 0x20);
		timerWaitUs(1000);

		i2c_write_write(0x2A, 0x31);
		timerWaitUs(1000);

}

void magnetic_detection()
{
i2c_write_write(0x54, 0x02);
i2c_write_write(0x55, 0xff);
i2c_write_write(0x52, 0xCA);
i2c_write_write(0x5A, 0x0A);
i2c_write_write(0x5B, 0x1F);
i2c_write_write(0x2C, 0x00);
}

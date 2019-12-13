/*
 * @filename 	: i2c.h
 * @description	: contains headers to support i2c.c
 * @author		: Puneet Bansal
 */

#include <i2cspm.h>
#include "em_gpio.h"
#include "gpio.h"
#include "em_cmu.h"
#include <em_i2c.h>


void i2c_init();
void i2c_transfer();
void i2c_write(I2C_TransferSeq_TypeDef);
uint16_t i2c_read(I2C_TransferSeq_TypeDef);
uint16_t i2c_write_read(uint8_t, uint8_t len);
void i2c_write_write(uint16_t , uint16_t);
void i2c_write_new(uint8_t,uint8_t);

#define GEEKY_PANDA_I2C 1

#if GEEKY_PANDA_I2C
	#define SCL_PORT gpioPortF
	#define SCL_PIN 7
	#define SDA_PORT gpioPortF
	#define SDA_PIN 6
	#define NO_HOLD_MASTER_TEMP 0x0d//0XE3;
	#define ACC_SLAVE_ADD 0X1E //sa0 sa1 00;
#else
	#define ACC_SLAVE_ADD 0X1F
#endif
#define SLAVE_ADD 0x40

#define NO_HOLD_MASTER_TEMP 0x0d//0XE3;
#define WHO_AM_I_ACC 0X0D

uint16_t data,len;


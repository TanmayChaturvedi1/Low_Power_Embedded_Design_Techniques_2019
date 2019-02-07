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

#define SCL_PORT gpioPortC
#define SCL_PIN 10

#define SDA_PORT gpioPortC
#define SDA_PIN 11
#define SLAVE_ADD 0x40
#define NO_HOLD_MASTER_TEMP 0XE3;

uint16_t data,len;


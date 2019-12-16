/*
 * @filename	: FXOS8700CQ.c
 * @description	: This file contains functions to perform motion and magnetic detection from FXOS8700CQ
 * @author 		: Puneet Bansal
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 * 				  FXOS8700CQ Datasheet : https://www.nxp.com/docs/en/data-sheet/FXOS8700CQ.pdf
 * 				  Motion Detection : https://www.nxp.com/docs/en/application-note/AN4070.pdf
 * 				  Magnetic Detection : https://community.nxp.com/thread/460414
 */

#include <src/FXOS8700CQ.h>
#include "main.h"


void motion_detection()
{
	//Set the device in 50 Hz ODR, Standby
	i2c_write_new(0x2A, 0X08);
	timerWaitUs(10000);
	timerWaitUs(10000);

	//Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling X,
	//Y, Z and the Latch
	i2c_write_write(0x15, 0XD8);
	timerWaitUs(1000);

	//setting threshold for resulting acceleration <0.2g
	i2c_write_write(0x17, 0X30);
	timerWaitUs(1000);

	//setting the debounce counter to 120ms
	i2c_write_write(0x18, 0X0A);
	timerWaitUs(1000);

	//Enabling motion/freefall interrupt in the system (ctrl_reg4)
	i2c_write_write(0x2D, 0X04);
	timerWaitUs(1000);

	//Route the interrupt function to INT2 hardware pin (ctrl_reg4)
	i2c_write_write(0x2E, 0X04);
	timerWaitUs(1000);

	//putting the device in active mode
	uint16_t val = i2c_write_read(0X2A, 1);
	timerWaitUs(1000);
	i2c_write_write(0x2A, (uint8_t)val|0x01);
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

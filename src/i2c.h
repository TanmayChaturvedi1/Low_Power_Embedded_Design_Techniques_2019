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
#define NFC_SLAVE_ADD 0X55
uint16_t data,len;





/* The functions required to communicate with the NFC module */
/*********************************************************************************
 * Function name: nfc_i2c_init()                                                 *
 * Description: Function initializes i2c0 for communication with the NFC module  *
 * @param: void                                                                  *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_init();


/*********************************************************************************
 * Function name: nfc_i2c_read_data_spm()                                        *
 * Description: Function uses the I2CSPM functions to read the data from the     *
 * 				NFC EEPROM memory from the sector passed as a parameter.         *
 * @param: 	uint8_t (addr)   - The slave address of the NFC module.              *
 * 			uint8_t (sector) - The sector to read from.                          *
 * @return: uint8_t* 		 - Returns the pointer to the data fetched from NFC  *
*********************************************************************************/
uint8_t* nfc_i2c_read_data_spm(uint8_t, uint8_t);


/*********************************************************************************
 * Function name: nfc_i2c_write_data_spm()                                       *
 * Description: Function uses the I2CSPM functions to write the data to the      *
 * 				NFC EEPROM memory from the sector and the data to write          *
 * 				passed as a parameter.                                           *
 * @param: 	uint8_t (addr)   		- The slave address of the NFC module.       *
 * 			uint8_t (sector) 		- The sector to write to.                    *
 * 			uint8_t* (write_data)	- The pointer to the data to be written.     *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_write_data_spm(uint8_t, uint8_t, uint8_t*);


/*********************************************************************************
 * Function name: nfc_i2c_write_data_to_nfc()                                    *
 * Description: Function passes the appropriate sectors and data to write to the *
 * 				nfc_i2c_write_data_spm function and writes the data to the module*
 * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
 * 			uint8_t (max_sector) 		- Number of sectors to write             *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_write_data_to_nfc(uint8_t address, uint8_t max_sectors);


///*********************************************************************************
// * Function name: set_temperature_threshold()                                    *
// * Description: Function returns the temperature threshold.                      *
// * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
// * @return: uint8_t 					- The temperature threshold.             *
// ********************************************************************************/
//uint8_t set_temperature_threshold(uint8_t address);
//
//
///*********************************************************************************
// * Function name: set_humidity_threshold()                                       *
// * Description: Function returns the temperature threshold.                      *
// * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
// * @return: uint8_t 					- The humidity threshold.                *
// ********************************************************************************/
//uint8_t set_humidity_threshold(uint8_t address);
//

/*********************************************************************************
 * Function name: nfc_get_all_the_written_values()                               *
 * Description: Function returns the temperature and humidity threshold and      *
 * 				writes the origin timestamp to the NFC data structure.           *
 * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
 * @return: uint8_t* (temperature_th)	- The temperature threshold.             *
 * @return: uint8_t* (humidity_th) 		- The humidity threshold.                *
 ********************************************************************************/
void nfc_get_all_the_written_values(uint8_t address, uint8_t* temperature_th, uint8_t* humidity_th);



/***********************************NFC FILES************************/

/* The functions required to communicate with the NFC module */
/*********************************************************************************
 * Function name: nfc_i2c_init()                                                 *
 * Description: Function initializes i2c0 for communication with the NFC module  *
 * @param: void                                                                  *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_init();


/*********************************************************************************
 * Function name: nfc_i2c_read_data_spm()                                        *
 * Description: Function uses the I2CSPM functions to read the data from the     *
 * 				NFC EEPROM memory from the sector passed as a parameter.         *
 * @param: 	uint8_t (addr)   - The slave address of the NFC module.              *
 * 			uint8_t (sector) - The sector to read from.                          *
 * @return: uint8_t* 		 - Returns the pointer to the data fetched from NFC  *
*********************************************************************************/
uint8_t* nfc_i2c_read_data_spm(uint8_t, uint8_t);


/*********************************************************************************
 * Function name: nfc_i2c_write_data_spm()                                       *
 * Description: Function uses the I2CSPM functions to write the data to the      *
 * 				NFC EEPROM memory from the sector and the data to write          *
 * 				passed as a parameter.                                           *
 * @param: 	uint8_t (addr)   		- The slave address of the NFC module.       *
 * 			uint8_t (sector) 		- The sector to write to.                    *
 * 			uint8_t* (write_data)	- The pointer to the data to be written.     *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_write_data_spm(uint8_t, uint8_t, uint8_t*);


/*********************************************************************************
 * Function name: nfc_i2c_write_data_to_nfc()                                    *
 * Description: Function passes the appropriate sectors and data to write to the *
 * 				nfc_i2c_write_data_spm function and writes the data to the module*
 * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
 * 			uint8_t (max_sector) 		- Number of sectors to write             *
 * @return: void                                                                 *
 ********************************************************************************/
void nfc_i2c_write_data_to_nfc(uint8_t address, uint8_t max_sectors);


///*********************************************************************************
// * Function name: set_temperature_threshold()                                    *
// * Description: Function returns the temperature threshold.                      *
// * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
// * @return: uint8_t 					- The temperature threshold.             *
// ********************************************************************************/
//uint8_t set_temperature_threshold(uint8_t address);
//
//
///*********************************************************************************
// * Function name: set_humidity_threshold()                                       *
// * Description: Function returns the temperature threshold.                      *
// * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
// * @return: uint8_t 					- The humidity threshold.                *
// ********************************************************************************/
//uint8_t set_humidity_threshold(uint8_t address);
//

/*********************************************************************************
 * Function name: nfc_get_all_the_written_values()                               *
 * Description: Function returns the temperature and humidity threshold and      *
 * 				writes the origin timestamp to the NFC data structure.           *
 * @param: 	uint8_t (address)   		- The slave address of the NFC module.   *
 * @return: uint8_t* (temperature_th)	- The temperature threshold.             *
 * @return: uint8_t* (humidity_th) 		- The humidity threshold.                *
 ********************************************************************************/
void nfc_get_all_the_written_values(uint8_t address, uint8_t* temperature_th, uint8_t* humidity_th);

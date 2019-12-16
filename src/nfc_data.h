/*
 * @filename	: nfc_data.h
 * @description	: This file contains functions to configure NFC peripheral.
 * @author 		: Nachiket Kelkar,Puneet Bansal, Tanmay Chaturvedi.
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 *                Assignments developed for the course IoT Embedded Firmware.
 */

#ifndef SRC_NFC_DATA_H_
#define SRC_NFC_DATA_H_

#include "stdint.h"

// Defines to know the data
#define TEMPERATURE_EXCEEDED 	1
#define HUMIDITY_EXCEEDED 	   	2
#define TOPPLE					3
#define DROP					4
#define FREE_FALLS				5
#define MAGNETIC_INTERFERENCE 	6


/***************************************************************************************
 * Function name: nfc_data_update()                                            	       *
 * Description: This function updates the nfc_data data structure which write the      *
 * 				time stamp and the event to the data structure which is used to update *
 * 				the NFC EEPROM for sending statistics.								   *
 * @param: uint32_t(time) - Time stamp, uint8_t(event) - Event that was triggered      *                                                                    *
 * @return: void                                                                       *
 **************************************************************************************/
void nfc_data_update(uint32_t, uint8_t);


/***************************************************************************************
 * Function name: update_header()                                            	       *
 * Description: The NFC NDEF messages needs the header to know the number of bytes     *
 * 				of payload to read. This fucntion updates the header information when  *
 * 				the data is written in the data structure.							   *
 * @param: uint8_t(sector) - Sector written, uint8_t(section) - Section written        *                                                                    *
 * @return: void                                                                       *
 **************************************************************************************/
void update_header(uint8_t, uint8_t);

#endif /* SRC_NFC_DATA_H_ */

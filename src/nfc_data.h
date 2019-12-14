/*
 * nfc_data.h
 *
 *  Created on: Dec 3, 2019
 *      Author: nachi
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


void nfc_data_update(uint32_t, uint8_t);
void update_header(uint8_t, uint8_t);

#endif /* SRC_NFC_DATA_H_ */

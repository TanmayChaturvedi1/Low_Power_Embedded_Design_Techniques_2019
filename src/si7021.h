/*
 * @filename 	: si7021.h
 * @description	: contains headers to support si7021.c
 * @author		: Puneet Bansal
 */

#include <stdint.h>

void temp_get();
void motion_detection();
void freefall_detection();
void magnetic_magnitude_detection();
void auto_calliberation();
void magnetic_detection();

int16_t refMagX,refMagY,refMagZ,refMagXHi,refMagYHi,refMagZHi,refMagXLo,refMagYLo,refMagZLo;
int16_t magXMin,magYMin,magZMin,magXMax,magYMax,magZMax,magThresholdHi,magThresholdLo,magThreshold;

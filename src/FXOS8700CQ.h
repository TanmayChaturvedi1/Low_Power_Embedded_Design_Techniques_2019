/*
 * @filename 	: FXOS8700CQ.h
 * @description	: contains headers to support si7021.c
 * @author		: Puneet Bansal
 * @reference	: Silicon Labs SDK -https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg13/html/index.html
 * 				  FXOS8700CQ Datasheet : https://www.nxp.com/docs/en/data-sheet/FXOS8700CQ.pdf
 * 				  Motion Detection : https://www.nxp.com/docs/en/application-note/AN4070.pdf
 * 				  Magnetic Detection : https://community.nxp.com/thread/460414
 */

#include <stdint.h>

/*@param : none
 *@return : none
 *@description : function that uses i2c to setup the sensor to give interrupts on detecting motion.
 */
void motion_detection();

/*@param : none
 *@return : none
 *@description : function that uses i2c to setup the sensor to give interrupts on detecting magnetic interference.
 */
void magnetic_detection();


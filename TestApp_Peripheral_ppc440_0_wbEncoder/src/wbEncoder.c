/* wb-encoder.c - Wishbone encoder basic driver implementation
 * Copyright (C) 2010 Adrian Alonso <aalonso00@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/*
 * Include files
 */
#include "wbEncoder.h"

/*
 * Constant definitions
 */

/*
 * Function prototypes
 */

/*
 * wbEncoder_CfgInitialize 
 */

int wbEncoder_CfgInitialize (wbEncoder *instPrt, wbEncoder_Config *config,
							 u32 effectiveAddr)
{
	Xil_AssertNonvoid (instPrt != NULL);
	Xil_AssertNonvoid (config != NULL);

	instPrt->baseAddress = effectiveAddr;
	instPrt->interruptPresent = config->interruptPresent;
	instPrt->isReady = XIL_COMPONENT_IS_READY;
	instPtr->readData = 0;
	instPrt->readDelta = 0;
	
	return XST_SUCCESS;	
}

/*
 * Look the device configuration in ConfigTable
 */

wbEncoder_Config *wbEncoder_LookupConfig (u16 deviceID)
{
	wbEncoder_Config *configPrt = NULL;
	int index;

	for (index=0; index < XPAR_PLBV46_2_WB_ENCODER_NUM_INSTANCES; index++) {
		if (wbEncoder_ConfigTable[index].deviceId == deviceId) {
			configPtr = &wbEncoder_ConfigTable[index];
			break;
		}
	}

	return configPtr;
}


/*
 * wbEncoder initialize function
 */
int wbEncoder_Initialize (wbEncoder *instPrt, u16 deviceId)
{
	wbEncoder_Config *configPrt;
	/* valid instPtr */
	Xil_AssertNonvoid (instPtr != NULL);

	configPtr = wbEncoder_LookupConfig (deviceId);
	if (configPtr == (wbEncoder_Config *) NULL) {
		instPtr->isReady = 0;
		instPtr->readData = 0;
		instPrt->readDelta = 0;
		return XST_DEVICE_NOT_FOUND;
	}

	return wbEncoder_CfgInitialize (instPtr, configPtr,
									configPtr->baseAddress);
}

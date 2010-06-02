/* TestApp_wb-encoder.c - Test App for wishbone encoder module
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
#include "xparameters.h"
#include "xstatus.h"
#include "stdio.h"

/*
 * Constant definitions
 */
#define WB_ENCODER_DEVICE_ID 

/*
 * Function prototypes
 */

int wbEncoderExample (u16 deviceId, u32 *dataRead);
void wbEncoderHandler (void *callbackHandler);


int
main (int argc, char **argv)
{
	int status;
	u32 inputData;
	
	status = wbEncoderExample (WB_ENCODER_DEVICE_ID, &inputData);

	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	return XST_SUCCESS;
}

int wbEncoderExample (u16 deviceId, u32 *dataRead)
{
	int status;

	status = 
}

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
#include "wbEencoder.h"

/*
 * Constant definitions
 */
#define WB_ENCODER_DEVICE_ID XPAR_PLBV46_2_WB_ENCONDER_0_DEVICE_ID

/* Global wbEncoder instance */
static wbEncoder wbEncoderInst;
/* Interrupt controller instance */
static XIntc interruptCtlr;

/*
 * Function prototypes
 */

int wbEncoderExample(wbEncoder *instPtr, u16 deviceId);
void wbEncoderHandler(void *callbackHandler);

int
main (int argc, char **argv)
{
	int status;
	
	status = wbEncoderExample (&wbEncoderInst, WB_ENCODER_DEVICE_ID);

	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	return XST_SUCCESS;
}

int wbEncoderExample (wbEncoder *instPrt, u16 deviceId)
{
	int status;

	status = wbEncoder_Initialize(instPtr, deviceId);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    wbEncoder_InterruptGlobalEnable(wbEncoderInst);

    for(;;) {
                
    }

    return XST_SUCCESS;
}

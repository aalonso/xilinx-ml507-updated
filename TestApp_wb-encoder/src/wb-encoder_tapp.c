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
#include "stdio.h"
#include "wb-encoder.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xintc.h"


/*
 * Constant definitions
 */
#define ANTICLOCKWISE	0
#define CLOCKWISE		1
#define KEYPRESSED		2


/* direction */
int direc;
/*
 * Function prototypes
 */

int wbEncoderExample(wbEncoder *instPtr, u16 deviceId,
					 XIntc *intcPtr, u16 intcId, u16 intcMask);
int wbEncoder_IntSetup(XIntc *intcPtr, wbEncoder *instPtr, 
						u16 deviceId, u16 intcId, u16 intcMask);
void wbEncoderIntHandler(void *callbackRef);

int
main(int argc, char **argv)
{
	int status;
	Xil_ICacheEnableRegion(0xc0000000);
	Xil_DCacheEnableRegion(0xc0000000);
	/* Global wbEncoder instance */
	static wbEncoder wbEncoderInst;
	/* Interrupt controller instance */
	static XIntc intcInst;
	
	status = wbEncoderExample(&wbEncoderInst,
							  (u16)(PLB2WB_ENCODER_DEVICEID),
							  &intcInst, (u16)(INTC_DEVICEID),
							  (u16)(PLB2WB_ENCODER_INTC_MASK));

	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	
	return XST_SUCCESS;
}

int wbEncoderExample(wbEncoder *instPtr u16 deviceId,
					 XIntc *intcPtr, u16 intcId, u16 intcMask)
{
	int status;

	status = wbEncoder_Initialize(instPtr, deviceId);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    } 

	status = wbEncoder_IntSetup(instPtr, deviceId, intcPtr,
								intcId, intcMask);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    } 

    for(;;) {
		if(direc == KEYPRESSED)
			break;		
    }

	/* Disable interrupts */
	wbEncoder_InterruptDisable(instPtr, 0x01);
	XIntc_Disable(intcPtr, intcId);
	
    return XST_SUCCESS;
}

int wbEncoder_Direction(wbEncoder *instPtr)
{
	int direction;
	switch(instPtr->readData) {
		case 0:
			switch(instPtr->readDelta) {
				case 1:
					direction = ANTICLOCKWISE;
					break;
				case 2:
					direction = CLOCKWISE;
					break;
				case 4:
					direction = KEYPRESSED;
					break;
			}
			break;
		case 1:
			switch(instPtr->readDelta) {
				case 0:
					direction = CLOCKWISE;
					break;
				case 3:
					direction = ANTICLOCKWISE;
					break;
			}
			break;
		case 2:
			switch(instPtr->readDelta) {
				case 0:
					direction = ANTICLOCKWISE;
					break;
				case 1:
					direction = CLOCKWISE;
					break;
			}
			break;
		case 3:
			switch(instPtr->readDelta) {
				case 1:
					direction = CLOCKWISE;
					break;
				case 2:
					direction = ANTICLOCKWISE;
					break;
			}
			break;
		case 4:
			direction = KEYPRESSED;
			break;
	}

	return direction;
}


void wbEncoderIntHandler(void *callbackRef)
{
	u32 data;
	wbEncoder *instPtr = (wbEncoder *)(callbackRef);
	/* save prvious value */
	instPtr->readDelta = instPtr->readData;
	/* read data register */
	wbEncoder_ReadReg(instPtr->baseAddress, 
					  PLB2WB_DATA_OFFSET, data);
	
	instPtr->readData = data & 0xE0;

	direc = wbEncoder_Direction(instPtr);
	
	printf("direction = %d", direc);
	
	wbEncoder_InterruptClear(instPtr, 0x01);
}

int wbEncoder_IntSetup(XIntc *intcPtr, wbEncoder *instPtr, 
						u16 deviceId, u16 intcId, u16 intcMask)
{
	int status;

	/* Initialize interrupt controller */
	status = XIntc_Initialize(intcPtr, intcId);

	if (status != XST_SUCCESS )
		return status;
	/* Hook interrupt service rutine */
	status = XIntc_Connect(intcPtr, 
							intcId, 
							(XInterruptHandler)(wbEncoderIntHandler),
							instPrt);

	if (status != XST_SUCESS )
		return status;

	/* Enable wbEncoder Interrupts */
    wbEncoder_InterruptEnable(instPtr);
    wbEncoder_InterruptGlobalEnable(instPtr);

	/* Enable interrupt controller */
	XIntc_Enable(intcPtr, intcId);

	/* Initialize exception table */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, 
						(Xil_ExceptionHandler)XIntc_InterruptHandler,
						intcPtr);
	/* Enable exception table */
	Xil_ExceptionEnable();

	/* start interrupt controller */
	status = XIntc_Start(intcPtr, XIN_REAL_MODE);
	
	if (status != XST_SUCESS )
		return status;

	return XST_SUCCESS;
}

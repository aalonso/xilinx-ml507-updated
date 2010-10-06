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
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_exception.h"
#include "xintc.h"
#include "plb2wb_encoder.h"

/*
 * Constant definitions
 */
static wbEncoder wbEncoderInst;
static XIntc XintcInst;

/*
 * Function prototypes
 */

int wbEncoder_IntSetup(XIntc *intcPtr, wbEncoder *instPtr, 
						u16 deviceId, u16 intcId, u16 intcMask);

int
main(int argc, char **argv)
{
	int status;
	u32 regVal;
	Xil_ICacheEnableRegion(0xc0000000);
	Xil_DCacheEnableRegion(0xc0000000);
	/* Global wbEncoder instance */
	wbEncoder *instPtr;
	/* Interrupt controller instance */
	XIntc *intcPtr;
	
	instPtr = &wbEncoderInst;
	intcPtr = &XintcInst;

	xil_printf("\n\rInitializing plb2wb_encoder\n\r");

	status = PLB2WB_ENCODER_Initialize(instPtr);

    if (status != XST_SUCCESS) 
        return XST_FAILURE; 
	
	xil_printf("Interrupt setup\n\r");

	status = wbEncoder_IntSetup(intcPtr, instPtr, 
								XPAR_PLB2WB_ENCODER_0_DEVICE_ID,
								XPAR_INTC_0_DEVICE_ID,
								XPAR_PLB2WB_ENCODER_0_IP2INTC_IRPT_MASK);
																		
    if (status != XST_SUCCESS)
        return XST_FAILURE;

	xil_printf("Wait loop\n\r");

    for(;;) {
		sleep(2); 
		regVal = PLB2WB_ENCODER_mReadReg(instPtr->baseAddress,
										 PLB2WB_ENCODER_SLV_REG0_OFFSET);
		xil_printf("regVal = 0x%x\n\r", regVal);

    }

	/* Disable interrupts */
	xil_printf("Interrupts disable\n\r");
	status = PLB2WB_ENCODER_DisableInterrupt(instPtr);
    
	if (status != XST_SUCCESS)
        return XST_FAILURE;

	xil_printf("Disable interrupt controller\n\r");
	XIntc_Disable(intcPtr, XPAR_INTC_0_DEVICE_ID);	

	xil_printf("Disable cache\n\r");
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	
	return XST_SUCCESS;
}

int wbEncoder_IntSetup(XIntc *intcPtr, wbEncoder *instPtr, 
						u16 deviceId, u16 intcId, u16 intcMask)
{
	int status;

	xil_printf("Initialize interrupt controller\n\r");
	/* Initialize interrupt controller */
	status = XIntc_Initialize(intcPtr, intcId);

	if (status != XST_SUCCESS )
		return status;
	
	/* Initialize exception table */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, 
						(Xil_ExceptionHandler)XIntc_DeviceInterruptHandler,
						/*(Xil_ExceptionHandler)XIntc_InterruptHandler,*/
						(void *) 0);
						/*intcPtr);*/
	
	/* Enable exception table */
	Xil_ExceptionEnable();
	
	/* start interrupt controller */
	xil_printf("Start interrupt controller\n\r");
	status = XIntc_Start(intcPtr, XIN_REAL_MODE);
	
	if (status != XST_SUCCESS )
		return status;

	/* Hook interrupt service rutine */
	xil_printf("Interrupt hook handler\n\r");
	status = XIntc_Connect(intcPtr, 
						   intcId, 
						   (XInterruptHandler)(PLB2WB_ENCODER_Intr_DefaultHandler),
						   instPtr);

	if (status != XST_SUCCESS )
		return status;

	/* Enable wbEncoder Interrupts */
	xil_printf("Enable wb_encoder interrupts\n\r");
	status = PLB2WB_ENCODER_EnableInterrupt(instPtr);
	
	if (status != XST_SUCCESS )
		return status;

	/* Enable interrupt controller */
	XIntc_Enable(intcPtr, intcId);



	return XST_SUCCESS;
}

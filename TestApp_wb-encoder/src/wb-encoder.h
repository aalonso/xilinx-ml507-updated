/* wbEncoder.h - Wishbone encoder header file
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

#ifndef __PLB2WB_ENCODER_H
#define __PLB2WB_ENCODER_H

/*
 * Include files
 */
#include "xstatus.h"
#include "xil_io.h"
#include "xil_types.h"
#include "xil_assert.h"
/*
 * Constant definitions
 */

#define PLB2WB_ENCODER_DEVICEID				XPAR_PLB2WB_ENCODER_0_DEVICE_ID
#define PLB2WB_ENCODER_BASEADDR				XPAR_PLB2WB_ENCODER_0_BASEADDR
#define PLB2WB_ENCODER_NUM_INSTANCES		XPAR_PLB2WB_ENCODER_NUM_INSTANCES
/* Registers offsets */
#define PLB2WB_ENCODER_DATA_OFFSET			PLB2PLB2WB_ENCONDER_SLV_REG0_OFFSET
/* Interrupt registers */
#define PLB2WB_ENCODER_INTC_MASK			XPAR_PLB2WB_ENCODER_0_IP2INTC_IRPT_MASK
#define PLB2WB_ENCODER_INTC_INTR			XPAR_XPS_INTC_0_PLB2WB_ENCONDER_0_IP2INTC_IRPT_INTR

#define INTC_DEVICEID						XPAR_XPS_INTC_0_DEVICE_ID
#define INTC_BASEADDR						XPAR_XPS_INTC_0_BASEADDR


/* macros for accessign registers */
#define wbEncoder_WriteReg(baseAddress, regOffset, data) \
		Xil_Out32((baseAddress) + (regOffset), (u32)(data))

#define wbEncoder_ReadReg(baseAddress, regOffset, data) \
		Xil_In32((baseAddress) + (regOffset), (u32)(data))


/*
 * wbEncoder_Config: Configuration information of device
 */

typedef struct {
	u16 deviceId;			/* device id */
	u32 baseAddress;		/* device base address */
} wbEncoder_Config;

/*
 * wbEncoder: driver instance data
 */

typedef struct {
	u16 deviceId;
	u32 baseAddress;
	u32 isReady;		/* device initialized */
	u32 readData;
	u32 readDelta;
} wbEncoder;

/*
 * Configuration table for device
 *
 */

wbEncoder_Config wbEncoder_ConfigTable[] =
{
	{
		PLB2WB_ENCODER_DEVICEID,
		PLB2WB_ENCODER_BASEADDR
	}
};

/*
 * Function prototypes
 */
/*
 * wbEncoder_CfgInitialize
 */
int wbEncoder_CfgIntialize (wbEncoder *instPtr, wbEncoder_Config *config,
							u32 effectiveAddr);


/*
 * Look the device configuration in ConfigTable
 */

wbEncoder_Config *wbEncoder_LookupConfig (u16 deviceId);

/*
 * wbEncoder initialize function
 */
int wbEncoder_Initialize(wbEncoder *instPrt, u16 deviceId);

/*
 * Global Interrupt eneble function
 */
void wbEncoder_InterruptGlobalEnable(wbEncoder *instPrt);

/*
 * Global Interrupt disable function
 */
void wbEncoder_InterruptGlobalDisable(wbEncoder *instPrt);

/*
 * Interrupt enable function
 */
void wbEncoder_InterruptEnable(wbEncoder *instPrt, u32 mask);

/*
 * Interrupt disable function
 */
void wbEncoder_InterruptDisable(wbEncoder *instPrt, u32 mask);

/*
 * Interrupt clear pending interrupt
 */
void wbEncoder_InterruptClear(wbEncoder *instPrt, u32 mask);

/*
 * Interrupt get eneabled interrupts
 */
u32 wbEncoder_InterruptGetEnabled(wbEncoder *instPrt);

/*
 * Interrupt get status
 */
u32 wbEncoder_InterruptGetStatus(wbEncoder *instPrt, u32 mask);

#endif		/* __PLB2WB_ENCODER_H */

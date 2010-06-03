/* wb-encoder.h - Wishbone encoder header file
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

#ifndef WB_ENCODER_H
#define WB_ENCODER_H

/*
 * Include files
 */
#include "xil_types.h"
#include "xstatus.h"
#include "xil_assert.h"

/*
 * Constant definitions
 */

/* Registers offsets */
#define WB_ENCODER_DATA_OFFSET			PLBV46_2_WB_ENCONDER_SLV_REG0_OFFSET
#define WB_ENCODER_CONTROL_OFFSET		PLBV46_2_WB_ENCONDER_SLV_REG1_OFFSET
#define WB_ENCODER_INT_CONTROL_OFFSET	PLBV46_2_WB_ENCONDER_SLV_REG2_OFFSET
#define WB_ENCODER_INT_STATUS_OFFSET	PLBV46_2_WB_ENCONDER_SLV_REG3_OFFSET

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
	int interruptPresent;	/* Interrupts supported in hw */
} wbEncoder_Config;

/*
 * wbEncoder: driver instance data
 */

typedef struct {
	u16 deviceId;
	u32 baseAddress;
	u32 isReady;		/* device initialized */
	int interruptPresent;
} wbEncoder;

/*
 * Configuration table for device
 *
 */

wbEncoder_Config wbEncoder_ConfigTable[] =
{
	{
		XPAR_PLBV46_2_WB_ENCONDER_0_DEVICE_ID,
		XPAR_PLBV46_2_WB_ENCONDER_0_BASEADDR,
		XPAR_PLBV46_2_WB_ENCONDER_0_INTERRUPT_PRESENT
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
 * wbEncoder Interrupt controller functions
 */

void wbEncoder_InterruptGlobalEnable(wbEncoder *instPrt);
void wbEncoder_InterruptGlobalDisable(wbEncoder *instPrt);
void wbEncoder_InterruptEnable(wbEncoder *instPrt, u32 mask);
void wbEncoder_InterruptDisable(wbEncoder *instPrt, u32 mask);
void wbEncoder_InterruptClear(wbEncoder *instPrt, u32 mask);
u32 wbEncoder_InterruptGetEnabled(wbEncoder *instPrt);
u32 wbEncoder_InterruptGetStatus(wbEncoder *instPrt, u32 mask);


#endif		/* WB_ENCODER_H */
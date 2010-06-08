/* wbEncoder_intc.c - Wishbone encoder basic driver implementation
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
 * wbEncoder Interrupt controller functions
 */

void wbEncoder_InterruptGlobalEnable(wbEncoder *instPrt)
{
	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);

	/* Enable interrupt source from user_logic */
	wbEncoder_WriteReg(wbEncoder->baseAddress,
						PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET,
						0x00000001);
	/* Enable interrupt all source from device */
	wbEncoder_WriteReg(wbEncoder->baseAddress,
						PLBV46_2_WB_ENCONDER_INTR_DIER_OFFSET,
						INTR_TERR_MASK | INTR_DPTO_MASK | INTR_IPIR_MASK);
	/* Enable global interrupts */
	wbEncoder_WriteReg(wbEncoder->baseAddress,
						PLBV46_2_WB_ENCONDER_INTR_DGIER_OFFSET,
						INTR_GIE_MASK);

}
void wbEncoder_InterruptGlobalDisable(wbEncoder *instPrt)
{
	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);

	/* Disable global interrupts */
	wbEncoder_WriteReg(wbEncoder->baseAddress,
						PLBV46_2_WB_ENCONDER_INTR_DGIER_OFFSET,
						0x0);
}

void wbEncoder_InterruptEnable(wbEncoder *instPrt, u32 mask)
{
	u32 reg_val;

	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);

	reg_val = wbEncoder_ReadReg(instPrt->baseAddress, 
								PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET);
	wbEncoder_WriteReg(instPrt->baseAddress, 
						PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET,
						reg_val | mask);
}

void wbEncoder_InterruptDisable(wbEncoder *instPrt, u32 mask)
{
	u32 reg_val;

	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);

	reg_val = wbEncoder_ReadReg(instPrt->baseAddress, 
								PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET);
	wbEncoder_WriteReg(instPrt->baseAddress, 
						PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET,
						reg_val & ~(mask));
}

void wbEncoder_InterruptClear(wbEncoder *instPrt, u32 mask)
{
	u32 reg_val;

	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);

	reg_val = wbEncoder_ReadReg(instPrt->baseAddress, 
								PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET);
	wbEncoder_WriteReg(instPrt->baseAddress,
						PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET,
						reg_val & mask);

}

u32 wbEncoder_InterruptGetEnabled(wbEncoder *instPrt)
{
	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);
	
	return wbEncoder_ReadReg(instPrt->baseAddress, 
							 PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET);
}

u32 wbEncoder_InterruptGetStatus(wbEncoder *instPrt, u32 mask)
{
	xil_AssertVoid(instPrt != NULL);
	xil_AssertVoid(instPrt->isReady == XIL_COMPONENT_IS_READY);
	xil_AssertVoid(instPrt->interruptPresent == TRUE);
	
	return wbEncoder_ReadReg(instPrt->baseAddress, 
							 PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET);
}

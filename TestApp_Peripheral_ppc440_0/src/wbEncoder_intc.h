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

#ifndef WB_ENCODER_INT_C_H
#define WB_ENCODER_INT_C_H

/*
 * Include files
 */
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xintc.h"
#include "wbEncoder.h"

/*
 * Constant definitions
 */
#define WB_ENCODER_INTC_MASK XPAR_PLBV46_2_WB_ENCONDER_0_IP2INTC_IRPT_MASK
#define WB_ENCODER_INTC_INTR XPAR_XPS_INTC_0_PLBV46_2_WB_ENCONDER_0_IP2INTC_IRPT_INTR

/*
 * Function prototypes
 */

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



#endif	/* WB_ENCODER_INT_C_H */


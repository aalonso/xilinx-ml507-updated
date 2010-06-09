/* wbEncoder_intc.h - Wishbone encoder interrupt header file
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

#ifndef WB_ENCODER_INTC_H
#define WB_ENCODER_INTC_H

/*
 * Include files
 */
#include "xil_types.h"
#include "xstatus.h"
#include "plbv46_2_wb_enconder.h"
#include "xparameters.h"

/*
 * Constant definitions
 */
#define WB_ENCODER_INTC_MASK 		XPAR_PLBV46_2_WB_ENCONDER_0_IP2INTC_IRPT_MASK
#define WB_ENCODER_INTC_INTR 		XPAR_XPS_INTC_0_PLBV46_2_WB_ENCONDER_0_IP2INTC_IRPT_INTR

/*
 * wbEncoder iterrupt global enable
 */
void wbEncoder_InterruptGlobalEnable(wbEncoder *instPrt);

/*
 * wbEncoder iterrupt global disable
 */
void wbEncoder_InterruptGlobalDisable(wbEncoder *instPrt);

/*
 * wbEncoder iterrupt enable
 */
void wbEncoder_InterruptEnable(wbEncoder *instPrt, u32 mask);

/*
 * wbEncoder iterrupt disable
 */
void wbEncoder_InterruptDisable(wbEncoder *instPrt, u32 mask);

/*
 * wbEncoder clear iterrupt
 */
void wbEncoder_InterruptClear(wbEncoder *instPrt, u32 mask);

/*
 * wbEncoder iterrupt getEnabled
 */
u32 wbEncoder_InterruptGetEnabled(wbEncoder *instPrt);

/*
 * wbEncoder iterrupt get status
 */
u32 wbEncoder_InterruptGetStatus(wbEncoder *instPrt, u32 mask);

#endif /* WB_ENCODER_INTC_H */

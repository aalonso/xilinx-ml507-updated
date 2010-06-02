/* $Id: xiic_intr.c,v 1.1.2.1 2009/11/27 09:20:51 svemula Exp $ */
/******************************************************************************
*
* (c) Copyright 2002-2009 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xiic_intr.c
*
* Contains interrupt functions of the XIic driver.  This file is required
* for the driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.01a rfp  10/19/01 release
* 1.01c ecm  12/05/02 new rev
* 1.01c rmm  05/14/03 Fixed diab compiler warnings relating to asserts.
* 1.03a ecm  06/22/06 Added a call to the status handler in the TxErrorHandler
*                     even if the Rx buffer pointer is not set. This fix is as
*                     a result of a Sony use model which did not set the Rx
*                     pointer while in Master mode so it checks if MSMS == 1.
* 1.13a wgr  03/22/07 Converted to new coding style.
* 2.00a sdm  10/22/09 Converted all register accesses to 32 bit access.
*		      Updated to use the HAL APIs/macros.
*		     Some of the macros have been renamed to remove _m from
*		     the name and Some of the macros have been renamed to be
*		     consistent, see the xiic_l.h file for further information.
* </pre>
*
******************************************************************************/


/***************************** Include Files *********************************/

#include "xiic.h"
#include "xiic_i.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions ******************/

/************************** Function Prototypes ****************************/

static void StubFunction(XIic *InstancePtr);
static void TxErrorHandler(XIic *InstancePtr);

/************************** Variable Definitions *****************************/

/* The following function pointers are used to help allow finer partitioning
 * of the driver such that some parts of it are optional. These pointers are
 * setup by functions in the optional parts of the driver.
 */
void (*XIic_AddrAsSlaveFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_NotAddrAsSlaveFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_RecvSlaveFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_SendSlaveFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_RecvMasterFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_SendMasterFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_ArbLostFuncPtr) (XIic *InstancePtr) = StubFunction;
void (*XIic_BusNotBusyFuncPtr) (XIic *InstancePtr) = StubFunction;

/*****************************************************************************/
/**
*
* This function is the interrupt handler for the XIic driver. This function
* should be connected to the interrupt system.
*
* Only one interrupt source is handled for each interrupt allowing
* higher priority system interrupts quicker response time.
*
* @param	InstancePtr is a pointer to the XIic instance to be worked on.
*
* @return	None.
*
* @internal
*
* The XIIC_INTR_ARB_LOST_MASK and XIIC_INTR_TX_ERROR_MASK interrupts must have
* higher priority than the other device interrupts so that the IIC device does
* not get into a potentially confused state. The remaining interrupts may be
* rearranged with no harm.
*
******************************************************************************/
void XIic_InterruptHandler(void *InstancePtr)
{
	u32 Status;
	u32 IntrStatus;
	u32 IntrPending;
	u32 IntrEnable;
	XIic *IicPtr = NULL;
	u32 Clear = 0;

	/*
	 * Verify that each of the inputs are valid.
	 */
	Xil_AssertVoid(InstancePtr != NULL);

	/*
	 * Convert the non-typed pointer to an IIC instance pointer
	 */
	IicPtr = (XIic *) InstancePtr;

	/*
	 * Get the interrupt Status.
	 */
	IntrPending = XIic_ReadIisr(IicPtr->BaseAddress);
	IntrEnable = XIic_ReadIier(IicPtr->BaseAddress);
	IntrStatus = IntrPending & IntrEnable;

	/*
	 * Do not processes a devices interrupts if the device has no
	 * interrupts pending or the global interrupts have been disabled.
	 */
	if ((IntrStatus == 0) |
		(XIic_IsIntrGlobalEnabled(IicPtr->BaseAddress) == FALSE)) {
		return;
	}

	/*
	 * Update interrupt stats and get the contents of the status register.
	 */
	IicPtr->Stats.IicInterrupts++;
	Status = XIic_ReadReg(IicPtr->BaseAddress, XIIC_SR_REG_OFFSET);

	/*
	 * Service requesting interrupt.
	 */
	if (IntrStatus & XIIC_INTR_ARB_LOST_MASK) {
		/* Bus Arbritration Lost */

		IicPtr->Stats.ArbitrationLost++;
		XIic_ArbLostFuncPtr(IicPtr);

		Clear = XIIC_INTR_ARB_LOST_MASK;
	} else if (IntrStatus & XIIC_INTR_TX_ERROR_MASK) {
		/* Transmit errors (no acknowledge) received */

		IicPtr->Stats.TxErrors++;
		TxErrorHandler(IicPtr);

		Clear = XIIC_INTR_TX_ERROR_MASK;
	} else if (IntrStatus & XIIC_INTR_NAAS_MASK) {
		/* Not Addressed As Slave */

		XIic_NotAddrAsSlaveFuncPtr(IicPtr);
		Clear = XIIC_INTR_NAAS_MASK;
	} else if (IntrStatus & XIIC_INTR_RX_FULL_MASK) {
		/* Receive register/FIFO is full */

		IicPtr->Stats.RecvInterrupts++;

		if (Status & XIIC_SR_ADDR_AS_SLAVE_MASK) {
			XIic_RecvSlaveFuncPtr(IicPtr);
		} else {
			XIic_RecvMasterFuncPtr(IicPtr);
		}

		Clear = XIIC_INTR_RX_FULL_MASK;
	} else if (IntrStatus & XIIC_INTR_AAS_MASK) {
		/* Addressed As Slave */

		XIic_AddrAsSlaveFuncPtr(IicPtr);
		Clear = XIIC_INTR_AAS_MASK;
	} else if (IntrStatus & XIIC_INTR_BNB_MASK) {
		/* IIC bus has transitioned to not busy */

		/* Check if send callback needs to run */
		if (IicPtr->BNBOnly == TRUE) {
			XIic_BusNotBusyFuncPtr(IicPtr);
			IicPtr->BNBOnly = FALSE;
		} else {
			IicPtr->SendHandler(IicPtr->SendCallBackRef, 0);
		}

		Clear = XIIC_INTR_BNB_MASK;

		/* The bus is not busy, disable BusNotBusy interrupt */
		XIic_DisableIntr(IicPtr->BaseAddress, XIIC_INTR_BNB_MASK);

	} else if ((IntrStatus & XIIC_INTR_TX_EMPTY_MASK) ||
		 (IntrStatus & XIIC_INTR_TX_HALF_MASK)) {
		/* Transmit register/FIFO is empty or � empty */
		IicPtr->Stats.SendInterrupts++;

		if (Status & XIIC_SR_ADDR_AS_SLAVE_MASK) {
			XIic_SendSlaveFuncPtr(IicPtr);
		} else {
			XIic_SendMasterFuncPtr(IicPtr);
		}

		IntrStatus = XIic_ReadIisr(IicPtr->BaseAddress);
		Clear = IntrStatus & (XIIC_INTR_TX_EMPTY_MASK |
					  XIIC_INTR_TX_HALF_MASK);
	}

	/*
	 * Clear Interrupts.
	 */
	XIic_WriteIisr(IicPtr->BaseAddress, Clear);
}

/******************************************************************************
*
* This function fills the FIFO using the occupancy register to determine the
* available space to be filled. When the repeated start option is on, the last
* byte is withheld to allow the control register to be properly set on the last
* byte.
*
* @param	InstancePtr is a pointer to the XIic instance to be worked on.
*
* @param	Role indicates the role of this IIC device, a slave or a master,
*		on the IIC bus (XIIC_SLAVE_ROLE or XIIC_MASTER_ROLE).
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XIic_TransmitFifoFill(XIic *InstancePtr, int Role)
{
	u8 AvailBytes;
	int LoopCnt;
	int NumBytesToSend;

	/*
	 * Determine number of bytes to write to FIFO. Number of bytes that
	 * can be put into the FIFO is (FIFO depth) - (current occupancy + 1)
	 * When more room in FIFO than msg bytes put all of message in the FIFO.
	 */
	AvailBytes = IIC_TX_FIFO_DEPTH -
			(u8) (XIic_ReadReg(InstancePtr->BaseAddress,
					XIIC_TFO_REG_OFFSET) + 1);

	if (InstancePtr->SendByteCount > AvailBytes) {
		NumBytesToSend = AvailBytes;
	} else {
		/*
		 * More space in FIFO than bytes in message.
		 */
		if ((InstancePtr->Options & XII_REPEATED_START_OPTION) ||
			(Role == XIIC_SLAVE_ROLE)) {
			NumBytesToSend = InstancePtr->SendByteCount;
		} else {
			NumBytesToSend = InstancePtr->SendByteCount - 1;
		}
	}

	/*
	 * Fill FIFO with amount determined above.
	 */
	for (LoopCnt = 0; LoopCnt < NumBytesToSend; LoopCnt++) {
		XIic_WriteSendByte(InstancePtr);
	}
}

/*****************************************************************************/
/**
*
* This interrupt occurs four different ways: Two as master and two as slave.
* Master:
* <pre>
*  (1) Transmitter (IMPLIES AN ERROR)
*      The slave receiver did not acknowledge properly.
*  (2) Receiver (Implies Tx complete)
*      Interrupt caused by setting TxAck high in the IIC to indicate to the
*      the last byte has been transmitted.
* </pre>
*
* Slave:
* <pre>
*  (3) Transmitter (Implies Tx complete)
*      Interrupt caused by master device indicating last byte of the message
*      has been transmitted.
*  (4) Receiver (IMPLIES AN ERROR)
*      Interrupt caused by setting TxAck high in the IIC to indicate Rx
*      IIC had a problem - set by this device and condition already known
*      and interrupt is not enabled.
* </pre>
*
* This interrupt is enabled during Master send and receive and disabled
* when this device knows it is going to send a negative acknowledge (Ack = No).
*
* Signals user of Tx error via status callback sending: XII_TX_ERROR_EVENT
*
* When MasterRecv has no message to send and only receives one byte of data
* from the salve device, the TxError must be enabled to catch addressing
* errors, yet there is not opportunity to disable TxError when there is no
* data to send allowing disabling on last byte. When the slave sends the
* only byte the NOAck causes a Tx Error. To disregard this as no real error,
* when there is data in the Receive FIFO/register then the error was not
* a device address write error, but a NOACK read error - to be ignored.
* To work with or without FIFO's, the Rx Data interrupt is used to indicate
* data is in the Rx register.
*
* @param	InstancePtr is a pointer to the XIic instance to be worked on.
*
* @return	None.
*
******************************************************************************/
static void TxErrorHandler(XIic *InstancePtr)
{
	u32 IntrStatus;
	u32 CntlReg;

	/*
	 * When Sending as a slave, Tx error signals end of msg. Not Addressed
	 * As Slave will handle the callbacks. this is used to only flush
	 * the Tx fifo. The addressed as slave bit is gone as soon as the bus
	 * has been released such that the buffer pointers are used to determine
	 * the direction of transfer (send or receive).
	 */
	if (InstancePtr->RecvBufferPtr == NULL) {
		/*
		 * Master Receiver finished reading message. Flush Tx fifo to
		 * remove an 0xFF that was written to prevent bus throttling,
		 * and disable all transmit and receive interrupts.
		 */
		XIic_FlushTxFifo(InstancePtr);
		XIic_DisableIntr(InstancePtr->BaseAddress,
				  XIIC_TX_RX_INTERRUPTS);


		/*
		 * If operating in Master mode, call status handler to indicate
		 * NOACK occured.
		 */
		CntlReg = XIic_ReadReg(InstancePtr->BaseAddress,
						XIIC_CR_REG_OFFSET);
		if ((CntlReg & XIIC_CR_MSMS_MASK) != 0) {
			InstancePtr->StatusHandler(InstancePtr->
						   StatusCallBackRef,
						   XII_SLAVE_NO_ACK_EVENT);
		}
		return;
	}

	/*
	 * Data in the receive register from either master or slave receive
	 * When:slave, indicates master sent last byte, message completed.
	 * When:master, indicates a master Receive with one byte received. When
	 * a byte is in Rx reg then the Tx error indicates the Rx data was
	 * recovered normally Tx errors are not enabled such that this should
	 * not occur.
	 */
	IntrStatus = XIic_ReadIisr(InstancePtr->BaseAddress);
	if (IntrStatus & XIIC_INTR_RX_FULL_MASK) {
		/* Rx Reg/FIFO has data,  Disable Tx error interrupts */

		XIic_DisableIntr(InstancePtr->BaseAddress,
				  XIIC_INTR_TX_ERROR_MASK);
		return;
	}

	XIic_FlushTxFifo(InstancePtr);

	/*
	 * Disable and clear Tx empty, � empty, Rx Full or Tx error interrupts.
	 */
	XIic_DisableIntr(InstancePtr->BaseAddress, XIIC_TX_RX_INTERRUPTS);
	XIic_ClearIntr(InstancePtr->BaseAddress, XIIC_TX_RX_INTERRUPTS);

	/* Clear MSMS as on Tx error when Rxing, the bus will be
	 * stopped but MSMS bit is still set. Reset to proper state
	 */
	CntlReg = XIic_ReadReg(InstancePtr->BaseAddress, XIIC_CR_REG_OFFSET);
	CntlReg &= ~XIIC_CR_MSMS_MASK;
	XIic_WriteReg(InstancePtr->BaseAddress, XIIC_CR_REG_OFFSET, CntlReg);


	/*
	 * Set FIFO occupancy depth = 1 so that the first byte will throttle
	 * next recieve msg.
	 */
	XIic_WriteReg(InstancePtr->BaseAddress, XIIC_RFD_REG_OFFSET, 0);

	/*
	 * Call the event callback.
	 */
	InstancePtr->StatusHandler(InstancePtr->StatusCallBackRef,
				   XII_SLAVE_NO_ACK_EVENT);
}

/*****************************************************************************/
/**
*
* This function is a stub function that is used for the default function for
* events that are handled optionally only when the appropriate modules are
* linked in.  Function pointers are used to handle some events to allow
* some events to be optionally handled.
*
* @param	InstancePtr is a pointer to the XIic instance to be worked on.
*
******************************************************************************/
static void StubFunction(XIic *InstancePtr)
{
	Xil_AssertVoidAlways();
}

/*****************************************************************************
* Filename:          /home/aalonso/workspace/xlnx/xilinx-ml507-updated/drivers/plb2wb_encoder_v1_04_a/src/plb2wb_encoder.c
* Version:           1.04.a
* Description:       plb2wb_encoder Driver Source File
* Date:              Tue Oct  5 13:36:40 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "xil_assert.h"
#include "plb2wb_encoder.h"

/************************** Function Definitions ***************************/


/**
 *
 * Initialize wbEncoder structure with parameter definitions.
 *
 * @param   instPtr is a pointer to the wbEncoder struct.
 *
 * @return  XST_SUCCESS.
 *
 * @note    None.
 *
 */
int PLB2WB_ENCODER_Initialize(wbEncoder *instPtr)
{
	Xil_AssertNonvoid(instPtr != NULL);

	instPtr->deviceId = XPAR_PLB2WB_ENCODER_0_DEVICE_ID;
	instPtr->baseAddress = XPAR_PLB2WB_ENCODER_0_BASEADDR;
	instPtr->readData = 0;
	instPtr->readDelta = 0;
	instPtr->isReady = XIL_COMPONENT_IS_READY;

	return XST_SUCCESS;
}

/**
 *
 * Enable all possible interrupts from PLB2WB_ENCODER device.
 *
 * @param   instPtr is a pointer to the wbEncoder struct.
 *
 * @return  XST_SUCCESS.
 *
 * @note	None.
 *
 */
int PLB2WB_ENCODER_EnableInterrupt(wbEncoder *instPtr)
{
	Xuint32 baseaddr;
	Xil_AssertVoid(instPtr != NULL);
	Xil_AssertVoid(instPtr->isReady == XIL_COMPONENT_IS_READY);

	baseaddr = (Xuint32) instPtr->baseAddress;

	/* Enable all interrupt source from user logic */
	PLB2WB_ENCODER_mWriteReg(baseaddr,
							 PLB2WB_ENCODER_INTR_IPIER_OFFSET,
							 0x00000001);

	/* Set global interrupt enable */
	PLB2WB_ENCODER_mWriteReg(baseaddr,
							 PLB2WB_ENCODER_INTR_DGIER_OFFSET,
							 INTR_GIE_MASK);

	return XST_SUCCESS;
}


/**
 *
 * Disable all possible interrupts from PLB2WB_ENCODER device.
 *
 * @param   instPtr is a pointer to the wbEncoder struct.
 *
 * @return  XST_SUCCESS.
 *
 * @note	None.
 *
 */
int PLB2WB_ENCODER_DisableInterrupt(wbEncoder *instPtr)
{
	Xuint32 baseaddr, regVal;
	Xil_AssertVoid(instPtr != NULL);
	Xil_AssertVoid(instPtr->isReady == XIL_COMPONENT_IS_READY);

	baseaddr = (Xuint32) instPtr->baseAddress;
	
	/* Disable all interrupt source from user logic */
	PLB2WB_ENCODER_mWriteReg(baseaddr,
							 PLB2WB_ENCODER_INTR_IPIER_OFFSET,
							 0x00000000);

	regVal = PLB2WB_ENCODER_mReadReg(baseaddr,
									 PLB2WB_ENCODER_INTR_DGIER_OFFSET);
	/* Disable global interrupt */
	PLB2WB_ENCODER_mWriteReg(baseaddr,
							 PLB2WB_ENCODER_INTR_DGIER_OFFSET,
							 regVal & ~(INTR_GIE_MASK));
	return XST_SUCCESS;
}

/**
 * Clear interrupts from PLB2WB_ENCODER device.
 *
 * @param   instPtr is a pointer to the wbEncoder struct.
 *
 * @return  XST_SUCCESS.
 *
 * @note	None.
 *
 */
int PLB2WB_ENCODER_ClearInterrupt(wbEncoder *instPtr)
{
	Xuint32 baseaddr, regVal;
	Xil_AssertVoid(instPtr != NULL);
	Xil_AssertVoid(instPtr->isReady == XIL_COMPONENT_IS_READY);

	baseaddr = (Xuint32) instPtr->baseAddress;
	
	regVal = PLB2WB_ENCODER_mReadReg(baseaddr,
									 PLB2WB_ENCODER_INTR_IPISR_OFFSET);
	/* Clear interrupt */
	PLB2WB_ENCODER_mWriteReg(baseaddr,
							 PLB2WB_ENCODER_INTR_IPISR_OFFSET,
							 regVal);

	return XST_SUCCESS;
}

/**
 *
 * Example interrupt controller handler for PLB2WB_ENCODER device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the PLB2WB_ENCODER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PLB2WB_ENCODER_Intr_DefaultHandler(void *callbackRef)
{
	Xuint32 baseaddr, regVal;
	Xuint32 IntrStatus;
	Xuint32 IpStatus;
	wbEncoder *instPtr;

	instPtr = (wbEncoder *)(callbackRef);

	Xil_AssertVoid(instPtr != NULL);
	Xil_AssertVoid(instPtr->isReady == XIL_COMPONENT_IS_READY);
	baseaddr = (Xuint32) instPtr->baseAddress;

	xil_printf("User logic interrupt! \n\r");

	/* Save prevoius value */
	instPtr->readDelta = instPtr->readData;


	regVal = PLB2WB_ENCODER_mReadReg(baseaddr,
									 PLB2WB_ENCODER_SLV_REG0_OFFSET);

	instPtr->readData = regVal & 0xE0;

	xil_printf("data  = 0x%x \n\r", instPtr->readData);
	xil_printf("delta = 0x%x \n\r", instPtr->readDelta);
	
	PLB2WB_ENCODER_ClearInterrupt(instPtr);

}

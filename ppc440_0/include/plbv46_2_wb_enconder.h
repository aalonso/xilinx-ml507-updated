/*****************************************************************************
* Filename:          /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/drivers/plbv46_2_wb_enconder_v1_01_a/src/plbv46_2_wb_enconder.h
* Version:           1.01.a
* Description:       plbv46_2_wb_enconder Driver Header File
* Date:              Tue May 25 10:26:42 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PLBV46_2_WB_ENCONDER_H
#define PLBV46_2_WB_ENCONDER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 */
#define PLBV46_2_WB_ENCONDER_USER_SLV_SPACE_OFFSET (0x00000000)
#define PLBV46_2_WB_ENCONDER_SLV_REG0_OFFSET (PLBV46_2_WB_ENCONDER_USER_SLV_SPACE_OFFSET + 0x00000000)
#define PLBV46_2_WB_ENCONDER_SLV_REG1_OFFSET (PLBV46_2_WB_ENCONDER_USER_SLV_SPACE_OFFSET + 0x00000004)
#define PLBV46_2_WB_ENCONDER_SLV_REG2_OFFSET (PLBV46_2_WB_ENCONDER_USER_SLV_SPACE_OFFSET + 0x00000008)
#define PLBV46_2_WB_ENCONDER_SLV_REG3_OFFSET (PLBV46_2_WB_ENCONDER_USER_SLV_SPACE_OFFSET + 0x0000000C)

/**
 * Interrupt Controller Space Offsets
 * -- INTR_DISR  : device (peripheral) interrupt status register
 * -- INTR_DIPR  : device (peripheral) interrupt pending register
 * -- INTR_DIER  : device (peripheral) interrupt enable register
 * -- INTR_DIIR  : device (peripheral) interrupt id (priority encoder) register
 * -- INTR_DGIER : device (peripheral) global interrupt enable register
 * -- INTR_ISR   : ip (user logic) interrupt status register
 * -- INTR_IER   : ip (user logic) interrupt enable register
 */
#define PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET (0x00000100)
#define PLBV46_2_WB_ENCONDER_INTR_DISR_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define PLBV46_2_WB_ENCONDER_INTR_DIPR_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define PLBV46_2_WB_ENCONDER_INTR_DIER_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define PLBV46_2_WB_ENCONDER_INTR_DIIR_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define PLBV46_2_WB_ENCONDER_INTR_DGIER_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET (PLBV46_2_WB_ENCONDER_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

/**
 * Interrupt Controller Masks
 * -- INTR_TERR_MASK : transaction error
 * -- INTR_DPTO_MASK : data phase time-out
 * -- INTR_IPIR_MASK : ip interrupt requeset
 * -- INTR_RFDL_MASK : read packet fifo deadlock interrupt request
 * -- INTR_WFDL_MASK : write packet fifo deadlock interrupt request
 * -- INTR_IID_MASK  : interrupt id
 * -- INTR_GIE_MASK  : global interrupt enable
 * -- INTR_NOPEND    : the DIPR has no pending interrupts
 */
#define INTR_TERR_MASK (0x00000001UL)
#define INTR_DPTO_MASK (0x00000002UL)
#define INTR_IPIR_MASK (0x00000004UL)
#define INTR_RFDL_MASK (0x00000020UL)
#define INTR_WFDL_MASK (0x00000040UL)
#define INTR_IID_MASK (0x000000FFUL)
#define INTR_GIE_MASK (0x80000000UL)
#define INTR_NOPEND (0x80)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a PLBV46_2_WB_ENCONDER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PLBV46_2_WB_ENCONDER device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PLBV46_2_WB_ENCONDER_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define PLBV46_2_WB_ENCONDER_mWriteReg(BaseAddress, RegOffset, Data) \
 	xil_io_out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a PLBV46_2_WB_ENCONDER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PLBV46_2_WB_ENCONDER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 PLBV46_2_WB_ENCONDER_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLBV46_2_WB_ENCONDER_mReadReg(BaseAddress, RegOffset) \
 	xil_io_in32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from PLBV46_2_WB_ENCONDER user logic slave registers.
 *
 * @param   BaseAddress is the base address of the PLBV46_2_WB_ENCONDER device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void PLBV46_2_WB_ENCONDER_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 PLBV46_2_WB_ENCONDER_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define PLBV46_2_WB_ENCONDER_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	xil_io_out32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define PLBV46_2_WB_ENCONDER_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	xil_io_out32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define PLBV46_2_WB_ENCONDER_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	xil_io_out32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define PLBV46_2_WB_ENCONDER_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	xil_io_out32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))

#define PLBV46_2_WB_ENCONDER_mReadSlaveReg0(BaseAddress, RegOffset) \
 	xil_io_in32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG0_OFFSET) + (RegOffset))
#define PLBV46_2_WB_ENCONDER_mReadSlaveReg1(BaseAddress, RegOffset) \
 	xil_io_in32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG1_OFFSET) + (RegOffset))
#define PLBV46_2_WB_ENCONDER_mReadSlaveReg2(BaseAddress, RegOffset) \
 	xil_io_in32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG2_OFFSET) + (RegOffset))
#define PLBV46_2_WB_ENCONDER_mReadSlaveReg3(BaseAddress, RegOffset) \
 	xil_io_in32((BaseAddress) + (PLBV46_2_WB_ENCONDER_SLV_REG3_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Enable all possible interrupts from PLBV46_2_WB_ENCONDER device.
 *
 * @param   baseaddr_p is the base address of the PLBV46_2_WB_ENCONDER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PLBV46_2_WB_ENCONDER_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the PLBV46_2_WB_ENCONDER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PLBV46_2_WB_ENCONDER_Intr_DefaultHandler(void * baseaddr_p);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PLBV46_2_WB_ENCONDER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus PLBV46_2_WB_ENCONDER_SelfTest(void * baseaddr_p);

#endif /** PLBV46_2_WB_ENCONDER_H */

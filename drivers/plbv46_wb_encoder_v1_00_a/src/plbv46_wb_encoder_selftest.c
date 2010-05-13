/*****************************************************************************
* Filename:          /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/drivers/plbv46_wb_encoder_v1_00_a/src/plbv46_wb_encoder_selftest.c
* Version:           1.00.a
* Description:       Contains a diagnostic self-test function for the plbv46_wb_encoder driver
* Date:              Fri May  7 00:08:09 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "plbv46_wb_encoder.h"

/************************** Constant Definitions ***************************/


/************************** Variable Definitions ****************************/


/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PLBV46_WB_ENCODER instance to be worked on.
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
XStatus PLBV46_WB_ENCODER_SelfTest(void * baseaddr_p)
{
  int     Index;
  Xuint32 baseaddr;
  Xuint8  Reg8Value;
  Xuint16 Reg16Value;
  Xuint32 Reg32Value;
  
  /*
   * Check and get the device address
   */
  /*
   * Base Address maybe 0. Up to developer to uncomment line below.
  XASSERT_NONVOID(baseaddr_p != XNULL);
   */
  baseaddr = (Xuint32) baseaddr_p;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Write to user logic slave module register(s) and read back
   */
  xil_printf("User logic slave module test...\n\r");
  xil_printf("   - write 1 to slave register 0 word 0\n\r");
  PLBV46_WB_ENCODER_mWriteSlaveReg0(baseaddr, 0, 1);
  Reg32Value = PLBV46_WB_ENCODER_mReadSlaveReg0(baseaddr, 0);
  xil_printf("   - read %d from register 0 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 1 )
  {
    xil_printf("   - slave register 0 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 2 to slave register 1 word 0\n\r");
  PLBV46_WB_ENCODER_mWriteSlaveReg1(baseaddr, 0, 2);
  Reg32Value = PLBV46_WB_ENCODER_mReadSlaveReg1(baseaddr, 0);
  xil_printf("   - read %d from register 1 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 2 )
  {
    xil_printf("   - slave register 1 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 3 to slave register 2 word 0\n\r");
  PLBV46_WB_ENCODER_mWriteSlaveReg2(baseaddr, 0, 3);
  Reg32Value = PLBV46_WB_ENCODER_mReadSlaveReg2(baseaddr, 0);
  xil_printf("   - read %d from register 2 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 3 )
  {
    xil_printf("   - slave register 2 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 4 to slave register 3 word 0\n\r");
  PLBV46_WB_ENCODER_mWriteSlaveReg3(baseaddr, 0, 4);
  Reg32Value = PLBV46_WB_ENCODER_mReadSlaveReg3(baseaddr, 0);
  xil_printf("   - read %d from register 3 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 4 )
  {
    xil_printf("   - slave register 3 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - slave register write/read passed\n\n\r");

  /*
   * Enable all possible interrupts and clear interrupt status register(s)
   */
  xil_printf("Interrupt controller test...\n\r");
  Reg32Value = PLBV46_WB_ENCODER_mReadReg(baseaddr, PLBV46_WB_ENCODER_INTR_IPISR_OFFSET);
  xil_printf("   - IP (user logic) interrupt status : 0x%08x\n\r", Reg32Value);
  xil_printf("   - clear IP (user logic) interrupt status register\n\r");
  PLBV46_WB_ENCODER_mWriteReg(baseaddr, PLBV46_WB_ENCODER_INTR_IPISR_OFFSET, Reg32Value);
  Reg32Value = PLBV46_WB_ENCODER_mReadReg(baseaddr, PLBV46_WB_ENCODER_INTR_DISR_OFFSET);
  xil_printf("   - Device (peripheral) interrupt status : 0x%08x\n\r", Reg32Value);
  xil_printf("   - clear Device (peripheral) interrupt status register\n\r");
  PLBV46_WB_ENCODER_mWriteReg(baseaddr, PLBV46_WB_ENCODER_INTR_DISR_OFFSET, Reg32Value);
  xil_printf("   - enable all possible interrupt(s)\n\r");
  PLBV46_WB_ENCODER_EnableInterrupt(baseaddr_p);
  xil_printf("   - write/read interrupt register passed\n\n\r");

  return XST_SUCCESS;
}

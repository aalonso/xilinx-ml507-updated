/*****************************************************************************
* Filename:          /home/aalonso/workspace/xlnx/xilinx-ml507-updated/drivers/plb2wb_encoder_v1_02_a/src/plb2wb_encoder.c
* Version:           1.02.a
* Description:       plb2wb_encoder Driver Source File
* Date:              Mon Oct  4 13:25:42 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "plb2wb_encoder.h"

/************************** Function Definitions ***************************/

/**
 *
 * Enable all possible interrupts from PLB2WB_ENCODER device.
 *
 * @param   baseaddr_p is the base address of the PLB2WB_ENCODER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PLB2WB_ENCODER_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  PLB2WB_ENCODER_mWriteReg(baseaddr, PLB2WB_ENCODER_INTR_IPIER_OFFSET, 0x00000001);

  /*
   * Enable all possible interrupt sources from device.
   */
  PLB2WB_ENCODER_mWriteReg(baseaddr, PLB2WB_ENCODER_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  PLB2WB_ENCODER_mWriteReg(baseaddr, PLB2WB_ENCODER_INTR_DGIER_OFFSET, INTR_GIE_MASK);
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
void PLB2WB_ENCODER_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = PLB2WB_ENCODER_mReadReg(baseaddr, PLB2WB_ENCODER_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = PLB2WB_ENCODER_mReadReg(baseaddr, PLB2WB_ENCODER_INTR_IPISR_OFFSET);
    PLB2WB_ENCODER_mWriteReg(baseaddr, PLB2WB_ENCODER_INTR_IPISR_OFFSET, IpStatus);
  }

}


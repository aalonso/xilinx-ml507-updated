/*****************************************************************************
* Filename:          /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/drivers/plbv46_2_wb_enconder_v1_01_a/src/plbv46_2_wb_enconder.c
* Version:           1.01.a
* Description:       plbv46_2_wb_enconder Driver Source File
* Date:              Tue May 25 10:26:42 2010 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "plbv46_2_wb_enconder.h"

/************************** Function Definitions ***************************/

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
void PLBV46_2_WB_ENCONDER_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  PLBV46_2_WB_ENCONDER_mWriteReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_IPIER_OFFSET, 0x00000001);

  /*
   * Enable all possible interrupt sources from device.
   */
  PLBV46_2_WB_ENCONDER_mWriteReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  PLBV46_2_WB_ENCONDER_mWriteReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for PLBV46_2_WB_ENCONDER device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the PLBV46_2_WB_ENCONDER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PLBV46_2_WB_ENCONDER_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = PLBV46_2_WB_ENCONDER_mReadReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = PLBV46_2_WB_ENCONDER_mReadReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET);
    PLBV46_2_WB_ENCONDER_mWriteReg(baseaddr, PLBV46_2_WB_ENCONDER_INTR_IPISR_OFFSET, IpStatus);
  }

}


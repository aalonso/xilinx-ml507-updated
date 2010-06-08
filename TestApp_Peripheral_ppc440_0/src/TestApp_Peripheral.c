/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 12.1 EDK_MS1.53d
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/
 */


// Located in: ppc440_0/include/xparameters.h
#include "xparameters.h"

#include "xil_cache.h"

#include "stdio.h"

#include "xintc.h"
#include "xil_exception.h"
#include "intc_header.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "gpio_header.h"
#include "gpio_intr_header.h"
#include "iic_header.h"
#include "xemaclite.h"
#include "xemaclite_example.h"
#include "emaclite_header.h"
#include "emaclite_intr_header.h"
#include "xsysace.h"
#include "sysace_header.h"
#include "xwdttb.h"
#include "wdttb_header.h"
#include "wdttb_intr_header.h"
#include "xtmrctr.h"
#include "tmrctr_header.h"
#include "tmrctr_intr_header.h"
#include "wbEncoder.h"
#include "wbEncoder_intc.h"

#define GPIO_CHANNEL1 1

//====================================================

int main (void) {


   static XIntc intc;

   Xil_ICacheEnableRegion(0xc0000000);
   Xil_DCacheEnableRegion(0xc0000000);
   static XGpio DIP_Switches_8Bit_Gpio;
   static XEmacLite Ethernet_MAC_EmacLite;
   static XWdtTb xps_timebase_wdt_0_Wdttb;
   static XTmrCtr xps_timer_0_Timer;
   static wbEncoder wbEncoderInst;

   print("-- Entering main() --\r\n");


   {
      int status;
      
      print("\r\n Runnning IntcSelfTestExample() for xps_intc_0...\r\n");
      
      status = IntcSelfTestExample(XPAR_XPS_INTC_0_DEVICE_ID);
      
      if (status == 0) {
         print("IntcSelfTestExample PASSED\r\n");
      }
      else {
         print("IntcSelfTestExample FAILED\r\n");
      }
   } 
	
   {
       int Status;

       Status = IntcInterruptSetup(&intc, XPAR_XPS_INTC_0_DEVICE_ID);
       if (Status == 0) {
          print("Intc Interrupt Setup PASSED\r\n");
       } 
       else {
         print("Intc Interrupt Setup FAILED\r\n");
      } 
   }

   /*
    * Peripheral SelfTest will not be run for RS232_Uart_1
    * because it has been selected as the STDOUT device
    */



   {
      u32 status;
      
      print("\r\nRunning GpioOutputExample() for LEDs_8Bit...\r\n");

      status = GpioOutputExample(XPAR_LEDS_8BIT_DEVICE_ID,8);
      
      if (status == 0) {
         print("GpioOutputExample PASSED.\r\n");
      }
      else {
         print("GpioOutputExample FAILED.\r\n");
      }
   }


   {
      u32 status;
      
      print("\r\nRunning GpioOutputExample() for LEDs_Positions...\r\n");

      status = GpioOutputExample(XPAR_LEDS_POSITIONS_DEVICE_ID,5);
      
      if (status == 0) {
         print("GpioOutputExample PASSED.\r\n");
      }
      else {
         print("GpioOutputExample FAILED.\r\n");
      }
   }


   {
      u32 status;
      
      print("\r\nRunning GpioInputExample() for Push_Buttons_5Bit...\r\n");

      u32 DataRead;
      
      status = GpioInputExample(XPAR_PUSH_BUTTONS_5BIT_DEVICE_ID, &DataRead);
      
      if (status == 0) {
         xil_printf("GpioInputExample PASSED. Read data:0x%X\r\n", DataRead);
      }
      else {
         print("GpioInputExample FAILED.\r\n");
      }
   }


   {
      u32 status;
      
      print("\r\nRunning GpioInputExample() for DIP_Switches_8Bit...\r\n");

      u32 DataRead;
      
      status = GpioInputExample(XPAR_DIP_SWITCHES_8BIT_DEVICE_ID, &DataRead);
      
      if (status == 0) {
         xil_printf("GpioInputExample PASSED. Read data:0x%X\r\n", DataRead);
      }
      else {
         print("GpioInputExample FAILED.\r\n");
      }
   }
   {
      
      int Status;
        
      u32 DataRead;
      
      print(" Press button to Generate Interrupt\r\n");
      
      Status = GpioIntrExample(&intc, &DIP_Switches_8Bit_Gpio, \
                               XPAR_DIP_SWITCHES_8BIT_DEVICE_ID, \
                               XPAR_XPS_INTC_0_DIP_SWITCHES_8BIT_IP2INTC_IRPT_INTR, \
                               GPIO_CHANNEL1, &DataRead);
	
      if (Status == 0 ){
             if(DataRead == 0)
                print("No button pressed. \r\n");
             else
                print("Gpio Interrupt Test PASSED. \r\n"); 
      } 
      else {
         print("Gpio Interrupt Test FAILED.\r\n");
      }
	
   }


   {
      int status;
            
      
      print("\r\n Runnning IicSelfTestExample() for IIC_EEPROM...\r\n");
      
      status = IicSelfTestExample(XPAR_IIC_EEPROM_DEVICE_ID);
      
      if (status == 0) {
         print("IicSelfTestExample PASSED\r\n");
      }
      else {
         print("IicSelfTestExample FAILED\r\n");
      }
   }


   {
      int status;
      
      print("\r\nRunning EmacLitePolledExample() for Ethernet_MAC...\r\n");
      status = EmacLitePolledExample(XPAR_ETHERNET_MAC_DEVICE_ID);
      if (status == 0) {
         print("EmacLite Polled Example PASSED\r\n");
      }
      else {
         print("EmacLite Polled Example FAILED\r\n");
      }
   }
   {
      int Status;

      print("\r\n Running Interrupt Test  for Ethernet_MAC...\r\n");
      
      Status = EmacLiteIntrExample(&intc, &Ethernet_MAC_EmacLite, \
                               XPAR_ETHERNET_MAC_DEVICE_ID, \
                               XPAR_XPS_INTC_0_ETHERNET_MAC_IP2INTC_IRPT_INTR);
	
      if (Status == 0) {
         print("EmacLite Interrupt Test PASSED\r\n");
      } 
      else {
         print("EmacLite Interrupt Test FAILED\r\n");
      }

   }


   {
      int status;
      
      print("\r\nRunning SysAceSelfTestExample() for SysACE_CompactFlash...\r\n");
      status = SysAceSelfTestExample(XPAR_SYSACE_COMPACTFLASH_DEVICE_ID);
      if (status == 0) {
         print("SysAceSelfTestExample PASSED\r\n");
      }
      else {
         print("SysAceSelfTestExample FAILED\r\n");
      }
   }


   {
      int status;
      
      print("\r\n Runnning WdtTbSelfTestExample() for xps_timebase_wdt_0...\r\n");
      
      status = WdtTbSelfTestExample(XPAR_XPS_TIMEBASE_WDT_0_DEVICE_ID);
      
      if (status == 0) {
         print("WdtTbSelfTestExample PASSED\r\n");
      }
      else {
         print("WdtTbSelfTestExample FAILED\r\n");
      }
   }
        
   {
      int Status;
      
      print("\r\n Running Interrupt Test  for xps_timebase_wdt_0...\r\n");
      
      Status = WdtTbIntrExample(&intc, &xps_timebase_wdt_0_Wdttb, \
                                 XPAR_XPS_TIMEBASE_WDT_0_DEVICE_ID, \
                                 XPAR_XPS_INTC_0_XPS_TIMEBASE_WDT_0_WDT_INTERRUPT_INTR);
	
      if (Status == 0) {
         print("Wdttb Interrupt Test PASSED\r\n");
      } 
      else {
         print("Wdttb Interrupt Test FAILED\r\n");
      }


   }


   {
      int status;
      
      print("\r\n Running TmrCtrSelfTestExample() for xps_timer_0...\r\n");
      
      status = TmrCtrSelfTestExample(XPAR_XPS_TIMER_0_DEVICE_ID, 0x0);
      
      if (status == 0) {
         print("TmrCtrSelfTestExample PASSED\r\n");
      }
      else {
         print("TmrCtrSelfTestExample FAILED\r\n");
      }
   }
   {
      int Status;

      print("\r\n Running Interrupt Test  for xps_timer_0...\r\n");
      
      Status = TmrCtrIntrExample(&intc, &xps_timer_0_Timer, \
                                 XPAR_XPS_TIMER_0_DEVICE_ID, \
                                 XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR, 0);
	
      if (Status == 0) {
         print("Timer Interrupt Test PASSED\r\n");
      } 
      else {
         print("Timer Interrupt Test FAILED\r\n");
      }

   }

   {
		int status;
      
		print("\r\n Running Interrupt Test  for wbEncoder wheel...\r\n");

		status = wbEncoderExample(wbEncoderInst, WB_ENCODER_DEVICE_ID,
									&intc, XPAR_XPS_INTC_0_DEVICE_ID, 
									XPAR_PLBV46_2_WB_ENCONDER_0_IP2INTC_IRPT_MASK);
		if (status == 0) {
			print("Timer Interrupt Test PASSED\r\n");
		} 
		else {
			print("Timer Interrupt Test FAILED\r\n");
		}

   }

   print("-- Exiting main() --\r\n");
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}


##############################################################################
## Filename:          /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/drivers/plbv46_wb_encoder_v1_00_a/data/plbv46_wb_encoder_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Fri May  7 00:08:09 2010 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "plbv46_wb_encoder" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}

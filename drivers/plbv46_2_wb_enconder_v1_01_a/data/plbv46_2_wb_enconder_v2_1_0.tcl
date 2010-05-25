##############################################################################
## Filename:          /home/aalonso/workspace/ppc44x/xilinx-ml507-updated/drivers/plbv46_2_wb_enconder_v1_01_a/data/plbv46_2_wb_enconder_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Tue May 25 10:26:42 2010 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "plbv46_2_wb_enconder" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}

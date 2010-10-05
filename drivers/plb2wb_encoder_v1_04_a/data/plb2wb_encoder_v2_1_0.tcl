##############################################################################
## Filename:          /home/aalonso/workspace/xlnx/xilinx-ml507-updated/drivers/plb2wb_encoder_v1_04_a/data/plb2wb_encoder_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Tue Oct  5 13:36:40 2010 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "plb2wb_encoder" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}

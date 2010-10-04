//----------------------------------------------------------------------------
// user_logic.vhd - module
//----------------------------------------------------------------------------
//
// ***************************************************************************
// ** Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.            **
// **                                                                       **
// ** Xilinx, Inc.                                                          **
// ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
// ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
// ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
// ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
// ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
// ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
// ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
// ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
// ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
// ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
// ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
// ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
// ** FOR A PARTICULAR PURPOSE.                                             **
// **                                                                       **
// ***************************************************************************
//
//----------------------------------------------------------------------------
// Filename:          user_logic.vhd
// Version:           1.02.a
// Description:       User logic module.
// Date:              Mon Oct  4 13:25:34 2010 (by Create and Import Peripheral Wizard)
// Verilog Standard:  Verilog-2001
//----------------------------------------------------------------------------
// Naming Conventions:
//   active low signals:                    "*_n"
//   clock signals:                         "clk", "clk_div#", "clk_#x"
//   reset signals:                         "rst", "rst_n"
//   generics:                              "C_*"
//   user defined types:                    "*_TYPE"
//   state machine next state:              "*_ns"
//   state machine current state:           "*_cs"
//   combinatorial signals:                 "*_com"
//   pipelined or register delay signals:   "*_d#"
//   counter signals:                       "*cnt*"
//   clock enable signals:                  "*_ce"
//   internal version of output port:       "*_i"
//   device pins:                           "*_pin"
//   ports:                                 "- Names begin with Uppercase"
//   processes:                             "*_PROCESS"
//   component instantiations:              "<ENTITY_>I_<#|FUNC>"
//----------------------------------------------------------------------------

module user_logic
(
  // -- ADD USER PORTS BELOW THIS LINE ---------------
  enc_data,
  enc_clk,
  // -- ADD USER PORTS ABOVE THIS LINE ---------------

  // -- DO NOT EDIT BELOW THIS LINE ------------------
  // -- Bus protocol ports, do not add to or delete 
  Bus2IP_Clk,                     // Bus to IP clock
  Bus2IP_Reset,                   // Bus to IP reset
  Bus2IP_Addr,                    // Bus to IP address bus
  Bus2IP_CS,                      // Bus to IP chip select
  Bus2IP_RNW,                     // Bus to IP read/not write
  Bus2IP_Data,                    // Bus to IP data bus
  Bus2IP_BE,                      // Bus to IP byte enables
  Bus2IP_RdCE,                    // Bus to IP read chip enable
  Bus2IP_WrCE,                    // Bus to IP write chip enable
  IP2Bus_Data,                    // IP to Bus data bus
  IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
  IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
  IP2Bus_Error,                   // IP to Bus error response
  IP2Bus_IntrEvent                // IP to Bus interrupt event
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
); // user_logic

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
/* Wishbone bridge states */
parameter ST_IDLE  = 6'b100000;
parameter ST_READ  = 6'b010000;
parameter ST_WRITE = 6'b001000;
parameter ST_ACK   = 6'b000100;
parameter ST_RTY   = 6'b000010;
parameter ST_ERROR = 6'b000001;
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_SLV_DWIDTH                   = 32;
parameter C_NUM_REG                      = 1;
parameter C_NUM_INTR                     = 1;
parameter C_BASEADDR                     = 0;
parameter C_NUM_RTY                      = 4;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

// -- ADD USER PORTS BELOW THIS LINE -----------------
input      [0 : 2]                        enc_data;
input									  enc_clk;
// -- ADD USER PORTS ABOVE THIS LINE -----------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol ports, do not add to or delete
input                                     Bus2IP_Clk;
input                                     Bus2IP_Reset;
input      [0 : 31]                       Bus2IP_Addr;
input      [0 : 1]                        Bus2IP_CS;
input                                     Bus2IP_RNW;
input      [0 : C_SLV_DWIDTH-1]           Bus2IP_Data;
input      [0 : C_SLV_DWIDTH/8-1]         Bus2IP_BE;
input      [0 : C_NUM_REG-1]              Bus2IP_RdCE;
input      [0 : C_NUM_REG-1]              Bus2IP_WrCE;
output     [0 : C_SLV_DWIDTH-1]           IP2Bus_Data;
output                                    IP2Bus_RdAck;
output                                    IP2Bus_WrAck;
output                                    IP2Bus_Error;
output     [0 : C_NUM_INTR-1]             IP2Bus_IntrEvent;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic

  // Nets for user logic slave model s/w accessible register example
  /*reg        [0 : C_SLV_DWIDTH-1]           slv_reg0;*/
  wire       [0 : 0]                        slv_reg_write_sel;
  wire       [0 : 0]                        slv_reg_read_sel;
  reg        [0 : C_SLV_DWIDTH-1]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;
  reg                                       slv_error;
  reg [0:5]  state, next_state;
  wire write_req;
  wire read_req;
  /* Wishbone interface */  
  wire wb_clk;    
  wire wb_rst;
  wire wb_ack;
  reg wb_we;
  reg wb_cyc;
  reg wb_stb;
  wire [0:C_SLV_DWIDTH-1] wb_addr;
  wire [0:C_SLV_DWIDTH-1] wb_data_in;
  wire [0:C_SLV_DWIDTH-1] wb_data_out;
  wire enc_irq;
  /* Retry logic registers */
  reg [0:C_NUM_RTY-1] rty_count;
  reg rty_abort;
  reg rty_en;
  
  // --USER logic implementation added here

  // ------------------------------------------------------
  // Example code to read/write user logic slave model s/w accessible registers
  // 
  // Note:
  // The example code presented here is to show you one way of reading/writing
  // software accessible registers implemented in the user logic slave model.
  // Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  // to one software accessible register by the top level template. For example,
  // if you have four 32 bit software accessible registers in the user logic,
  // you are basically operating on the following memory mapped registers:
  // 
  //    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  //                     "1000"   C_BASEADDR + 0x0
  //                     "0100"   C_BASEADDR + 0x4
  //                     "0010"   C_BASEADDR + 0x8
  //                     "0001"   C_BASEADDR + 0xC
  // 
  // ------------------------------------------------------

  /* Wishbone complaint module instance */

  defparam wb_enc_inst.C_WB_DATAREG = C_BASEADDR + {C_SLV_DWIDTH {1'b0}};
  wb_encoder wb_enc_inst (
    .wb_clk_i (wb_clk),
    .wb_rst_i (wb_rst),
    .wb_we_i  (wb_we),
    .wb_cyc_i (wb_cyc),
    .wb_stb_i (wb_stb),
    .wb_ack_o (wb_ack),
    .wb_addr_i(wb_addr),
    .wb_data_i(wb_data_in),
    .wb_data_o(wb_data_out),
    .enc_data (enc_data),
	.enc_clk  (enc_clk),
    .irq_o    (enc_irq)    
  );  

  assign
    slv_reg_write_sel = Bus2IP_WrCE[0:0],
    slv_reg_read_sel  = Bus2IP_RdCE[0:0],
    slv_write_ack     = Bus2IP_WrCE[0],
    slv_read_ack      = Bus2IP_RdCE[0],
    /* Wishbone foward signals */
    wb_clk            = Bus2IP_Clk,
    wb_rst            = Bus2IP_Reset,
    wb_addr           = Bus2IP_Addr,    
    read_req          = | slv_reg_read_sel,
    write_req         = | slv_write_ack;

  /* Wishbone next state logic */
  always @(posedge Bus2IP_Clk)
  begin
    if(Bus2IP_Reset == 1)
        state = ST_IDLE;
    else
        state = next_state;
  end

  /* Wishbone bridge state machine */
  always @(state)
  begin
    case(state)
        ST_IDLE: begin   /* Idle state */
            wb_cyc <= 1'b0;
            wb_stb <= 1'b0;
            rty_en <= 1'b0;
            slv_error <= 1'b0;
            if (read_req == 1 && write_req == 0)
                next_state <= ST_READ;
            else if (read_req == 0 && write_req == 1)
                next_state <= ST_WRITE;
            else
                next_state <= ST_IDLE;
        end
        ST_READ: begin  /* Read request state */
            wb_cyc <= 1'b1;     /* Wishbone read request */
            wb_stb <= 1'b1;
            wb_we  <= 1'b0;
            next_state  <= ST_ACK;
        end
        ST_WRITE: begin  /* Write request state */
            wb_cyc <= 1'b1;     /* Wishbone write request */
            wb_stb <= 1'b1;
            wb_we  <= 1'b1;
            next_state  <= ST_ACK;
        end
        ST_ACK: begin    /* Ack state */
            if(wb_ack == 1)
                next_state <= ST_IDLE;
            else
                next_state <= ST_RTY;
        end
        ST_RTY: begin    /* Rerty state */
            rty_en <= 1'b1; /* Enable rty counter */
            if(rty_abort == 1)
                next_state <= ST_ERROR;
            else
                next_state <= ST_ACK;
        end
        ST_ERROR: begin  /* Error state */
            slv_error <= 1'b1;
            next_state <= ST_IDLE;
        end
        default :
            next_state <= ST_IDLE;
    endcase
  end /* Wishbone state machine */

  /* Wishbone retry logic */
  always @(posedge Bus2IP_Clk)
  begin
    if(Bus2IP_Reset == 1) begin
        rty_abort  <= 1'b0;
        rty_count  <= {C_NUM_RTY {1'b0}};
    end
    else begin
        if(rty_en == 0)
            rty_count <= {C_NUM_RTY {1'b0}};
        else
            rty_count <= rty_count + 1;
            
        rty_abort <= 1'b0;

        if(rty_count == C_NUM_RTY-1)
            rty_abort <= 1'b1;
    end
  end

  // implement slave model register(s)
  /* 
   * Wishbone wheel encoder 
   * Only implements a read only register
   */
  /*
  always @( posedge Bus2IP_Clk )
    begin: SLAVE_REG_WRITE_PROC

      if ( Bus2IP_Reset == 1 )
        begin
          slv_reg0 <= 0;
        end
      else
        case ( slv_reg_write_sel )
          1'b1 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  slv_reg0[bit_index] <= Bus2IP_Data[bit_index];
          default : ;
        endcase

    end */// SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or wb_data_out )
    begin: SLAVE_REG_READ_PROC

      case ( slv_reg_read_sel )
        1'b1 : slv_ip2bus_data <= wb_data_out;
        default : slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

  // ------------------------------------------------------------
  // drive IP to Bus signals
  // ------------------------------------------------------------

  assign IP2Bus_Data    = slv_ip2bus_data;
  assign IP2Bus_WrAck   = slv_write_ack | wb_ack;
  assign IP2Bus_RdAck   = slv_read_ack | wb_ack;
  assign IP2Bus_Error   = slv_error;
  assign IP2Bus_IntrEvent = enc_irq;

endmodule

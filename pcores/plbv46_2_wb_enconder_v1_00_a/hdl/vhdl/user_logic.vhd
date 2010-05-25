------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Wed May 12 20:09:47 2010 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here
library plbv46_2_wb_enconder_v1_00_a;
use plbv46_2_wb_enconder_v1_00_a.user_logic;
------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--   C_NUM_INTR                   -- Number of interrupt event
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Addr                  -- Bus to IP address bus
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
--   IP2Bus_IntrEvent             -- IP to Bus interrupt event
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    C_WB_DBUS_SIZE                 : integer              := 32;
    C_WB_ACCESS_TIMEOUT            : integer              := 16;
    C_WB_RETRY_TIMEOUT             : integer              := 256;
    C_WB_ACCESS_RETRIES            : integer              := 4;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 2;
    C_NUM_INTR                     : integer              := 1
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
	wb_encoder_in				   : in std_logic_vector(0 to C_WB_DBUS_SIZE-1);
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Addr                    : in  std_logic_vector(0 to 31);
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic;
    IP2Bus_IntrEvent               : out std_logic_vector(0 to C_NUM_INTR-1)
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute SIGIS : string;
  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic
  -- wishbone signals
  signal wb_clk_in   : std_logic;
  signal wb_rst_in   : std_logic;
  signal wb_cyc_in   : std_logic;
  signal wb_stb_in   : std_logic;
  signal wb_we_in    : std_logic;
  signal wb_ack_out  : std_logic;
  signal wb_err_in   : std_logic;
  signal wb_rty_in   : std_logic;
  signal wb_irq_out  : std_logic;
  signal wb_sel_out  : std_logic_vector(0 to (C_WB_DBUS_SIZE/8)-1);
  signal wb_addr_in  : std_logic_vector(0 to C_WB_DBUS_SIZE-1);
  signal wb_data_out : std_logic_vector(0 to C_WB_DBUS_SIZE-1);
  signal wb_data_in  : std_logic_vector(0 to C_WB_DBUS_SIZE-1);
  -- State Machine Declarations
  type state_type is (ST_IDLE, ST_ACCESS, ST_RETRY_STROBE, ST_RETRY, ST_ERROR, ST_DONE);
  signal curr_st        : state_type;
  signal next_st        : state_type;
  -- Bus Ack Decode
  signal wb_rdack       : std_logic;
  signal wb_wrack       : std_logic;
  -- Timer used to track bus error condition and retry timouts.
  signal timer_en       : std_logic;
  signal timer_cnt      : std_logic_vector(0 to 7);
  -- Counter used to track number of retry attempts
  signal retry_iter     : std_logic_vector(0 to 1);
  signal retry_iter_rst : std_logic;
  signal retry_iter_en  : std_logic;
  -- Status Signals
  signal retry_expire   : std_logic;  -- Maximum Retries exceeded
  signal access_to      : std_logic;  -- Bus Error Detected
  signal retry_to       : std_logic;  -- Retry cycle completed


  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  -- signal slv_reg0                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  --  signal slv_reg1                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- signal slv_reg_write_sel              : std_logic_vector(0 to 1);
  -- signal slv_reg_read_sel               : std_logic_vector(0 to 1);
  -- signal slv_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- signal slv_read_ack                   : std_logic;
  -- signal slv_write_ack                  : std_logic;

  ------------------------------------------
  -- Signals for user logic interrupt example
  ------------------------------------------
  signal intr_counter                   : std_logic_vector(0 to C_NUM_INTR-1);

begin
    ----------------------------------------
    -- instantiate wb_encoder
    ----------------------------------------
    WB_ENCODER_I : entity plbv46_2_wb_enconder_v1_00_a.wb_encoder
    generic map
    (
     C_WB_DBUS_SIZE     => C_WB_DBUS_SIZE 
    )
    port map
    (
        wb_clk_in      => wb_clk_in,
        wb_rst_in      => wb_rst_in,
        wb_cyc_in      => wb_cyc_in,
        wb_stb_in      => wb_stb_in,
        wb_we_in       => wb_we_in,
        wb_addr_in     => wb_addr_in,
        wb_data_in     => wb_encoder_o,
        wb_data_out    => wb_data_out,
		wb_irq_out	   => wb_irq_out,
		wb_ack_out     => wb_ack_out
    )

  --USER logic implementation added here
  --
  -- We are not buffering these signals to the WB Bus.
  -- Nor are we running the clock at a slower rate than the PLB Bus.
  wb_clk_in <= Bus2IP_Clk;
  wb_rst_in <= Bus2IP_Reset;
  -- These can probably be treated as multi-cycle paths
  -- Possibly will add in a Pipeline stage (user selectable?)
  wb_addr_in <= Bus2IP_Addr;
  wb_data_out <= Bus2IP_Data;
  wb_sel_out  <= Bus2IP_BE;
  wb_we_in   <= not Bus2IP_RNW;

  -- Number of retry attempts
  process(Bus2IP_Clk) begin
    if (rising_edge(Bus2IP_Clk)) then
      if (retry_iter_rst = '1') then 
        retry_iter <= (others=>'0');
      elsif (retry_iter_en = '1') then
        retry_iter <= retry_iter + 1;
      end if;

      retry_expire <= '0';
      if (retry_iter = conv_std_logic_vector(C_WB_ACCESS_RETRIES-1,2)) then
        retry_expire <= '1';
      end if;
    end if;
  end process;
 
  -- Retry Wait Counter 
  process(Bus2IP_Clk) begin
    if (rising_edge(Bus2IP_Clk)) then
      if (timer_en = '0') then
        timer_cnt <= (others => '0');
      else
        timer_cnt <= timer_cnt + 1;
      end if;

      retry_to <= '0';
      if (timer_cnt = conv_std_logic_vector(C_WB_RETRY_TIMEOUT-1, 8)) then
        retry_to <= '1';
      end if;

      if (timer_cnt = conv_std_logic_vector(C_WB_ACCESS_TIMEOUT-1, 8)) then
        access_to <= '1';
      end if;
    end if;
  end process;
  
  -- WB Bridge State Machine (Next State Logic)
  IP2Bus_RdAck <= wb_rdack;
  IP2Bus_WrAck <= wb_wrack;

  process(curr_st, Bus2IP_CS ,wb_rty_in ,wb_ack_out, retry_to, access_to) begin
  	next_st <= curr_st;
  	timer_en <= '0';
    retry_iter_rst <= '0';
  	retry_iter_en <= '0';
    wb_stb_in <= '0';
    wb_cyc_in <= '0';
 	wb_rdack <= '0';
    wb_wrack <= '0';
 	IP2Bus_Error <= '0';

    case (curr_st) is
  	    when ST_IDLE =>
      	    retry_iter_rst <= '1';
  		    if (Bus2IP_CS(0) = '1') then
  	 	        next_st <= ST_ACCESS;
      	    end if;
        -- Access State
  		-- Completes when we receive either a RETRY, ACK or we timeout of the transaction.
      	-- Transaction timeout is setup by the user.
        when ST_ACCESS =>
  		    wb_stb_in <= '1';
  		   	wb_cyc_in <= '1';
      	    timer_en <= '1';
            if (wb_rty_in = '1') then
                next_st <= ST_RETRY_STROBE;
            elsif (wb_ack_out = '1') then
               	next_st <= ST_DONE;
            elsif (access_to = '1') then
                next_st <= ST_ERROR;
            end if;
      	-- Retry Strobe
  	    -- Simply used to reset timer and increment our retries.
      	-- We will also check to see if we have reached out limit of retries.
      	when ST_RETRY_STROBE =>
  	        retry_iter_en <= '1';
      	    if (retry_expire = '1') then
  	  		    next_st <= ST_ERROR;
      	  	else
  	      	    next_st <= ST_RETRY;
  	        end if;
        -- Retry
  	    -- Sit here and wait until we issues a WB Retry
  	    when ST_RETRY =>
      	    timer_en <= '1';
      	    if (retry_to = '1') then
  	            next_st <= ST_ACCESS;
  	        end if;
        -- Issue PLB Error
        when ST_ERROR =>
            IP2Bus_Error <= '1';
      	    wb_wrack <= not Bus2IP_RNW;
          	wb_rdack <= Bus2IP_RNW;
            next_st <= ST_IDLE;
  	  
        when ST_DONE =>
  	  	    wb_rdack <= Bus2IP_RNW;
      	  	wb_wrack <= not Bus2IP_RNW;
  	      	next_st <= ST_IDLE;
  	    end case;
    end process;

    -- WB Bridge State Machine (Current State Logic)
    process(Bus2IP_Clk) begin
  	    if (rising_edge(Bus2IP_Clk)) then
  		    if (Bus2IP_Reset = '1') then
  			    curr_st <= ST_IDLE;
  		    else
  			    curr_st <= next_st;
  		    end if;
  	    end if;
    end process;
   
  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  -- slv_reg_write_sel <= Bus2IP_WrCE(0 to 1);
  -- slv_reg_read_sel  <= Bus2IP_RdCE(0 to 1);
  -- slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1);
  -- slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1);

  -- implement slave model software accessible register(s)
  -- SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
  -- begin

  --   if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
  --     if Bus2IP_Reset = '1' then
  --       slv_reg0 <= (others => '0');
  --       slv_reg1 <= (others => '0');
  --     else
  --       case slv_reg_write_sel is
  --         when "10" =>
  --           for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
  --             if ( Bus2IP_BE(byte_index) = '1' ) then
  --               slv_reg0(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
  --             end if;
  --           end loop;
  --         when "01" =>
  --           for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
  --             if ( Bus2IP_BE(byte_index) = '1' ) then
  --               slv_reg1(byte_index*8 to byte_index*8+7) <= Bus2IP_Data(byte_index*8 to byte_index*8+7);
  --             end if;
  --           end loop;
  --         when others => null;
  --       end case;
  --     end if;
  --   end if;

  -- end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  -- SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0, slv_reg1 ) is
  -- begin

  --   case slv_reg_read_sel is
  --     when "10" => slv_ip2bus_data <= slv_reg0;
  --     when "01" => slv_ip2bus_data <= slv_reg1;
  --     when others => slv_ip2bus_data <= (others => '0');
  --   end case;

  -- end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to generate user logic interrupts
  -- 
  -- Note:
  -- The example code presented here is to show you one way of generating
  -- interrupts from the user logic. This code snippet infers a counter
  -- and generate the interrupts whenever the counter rollover (the counter
  -- will rollover ~21 sec @50Mhz).
  ------------------------------------------
  -- INTR_PROC : process( Bus2IP_Clk ) is
  --  constant COUNT_SIZE   : integer := 30;
  --  constant ALL_ONES     : std_logic_vector(0 to COUNT_SIZE-1) := (others => '1');
  --  variable counter      : std_logic_vector(0 to COUNT_SIZE-1);
  -- begin

  --   if ( Bus2IP_Clk'event and Bus2IP_Clk = '1' ) then
  --     if ( Bus2IP_Reset = '1' ) then
  --       counter := (others => '0');
  --       intr_counter <= (others => '0');
  --     else
  --       counter := counter + 1;
  --       if ( counter = ALL_ONES ) then
  --         intr_counter <= (others => '1');
  --       else
  --         intr_counter <= (others => '0');
  --       end if;
  --     end if;
  --   end if;

  -- end process INTR_PROC;

  -- IP2Bus_IntrEvent <= intr_counter;

  -- Foward interrups 
  IP2Bus_IntrEvent <= wb_irq_out;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  -- IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
  --                 (others => '0');

  -- IP2Bus_WrAck <= slv_write_ack;
  -- IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';

end IMP;

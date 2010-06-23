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
-- Version:           1.01.a
-- Description:       User logic.
-- Date:              Tue May 25 10:26:22 2010 (by Create and Import Peripheral Wizard)
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
library plbv46_2_wb_enconder_v1_01_a;
use plbv46_2_wb_enconder_v1_01_a.wb_encoder;

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
--   Bus2IP_CS                    -- Bus to IP chip select
--   Bus2IP_RNW                   -- Bus to IP read/not write
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
    C_NUM_REG                      : integer              := 4;
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
    Bus2IP_CS                      : in  std_logic_vector(0 to 1);
    Bus2IP_RNW                     : in  std_logic;
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
  -- Bus Ack Decode
  signal wb_rdack       : std_logic;
  signal wb_wrack       : std_logic;
  -- State machine signals
  type state_type is (StIdle, StWrReq, StRdReq, StAck);
  signal st_current : state_type;
  signal st_next	: state_type;
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  -- signal slv_reg0                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- signal slv_reg1                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- signal slv_reg2                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  -- signal slv_reg3                       : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_reg_write_sel              : std_logic_vector(0 to 3);
  signal slv_reg_read_sel               : std_logic_vector(0 to 3);
  signal slv_ip2bus_data                : std_logic_vector(0 to C_SLV_DWIDTH-1);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;
  signal writeReq						: std_logic;
  signal readReq						: std_logic;

  -----------------------------------------
  -- General pourpuse functions
  -----------------------------------------
  function or_reduce(l: std_logic_vector) return std_logic is
	variable v : std_logic := '0';
	begin
		for i in l'range loop v := v or l(i); end loop;
		return v;
	end;


begin
    ----------------------------------------
    -- instantiate wb_encoder
    ----------------------------------------
    WB_ENCODER_I : entity plbv46_2_wb_enconder_v1_01_a.wb_encoder
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
        wb_data_in     => wb_encoder_in,
        wb_data_out    => wb_data_out,
		wb_irq_out	   => wb_irq_out,
		wb_ack_out     => wb_ack_out
    );

	--USER logic implementation added here
	--
	-- Forward rst and clk signals
	wb_clk_in  <= Bus2IP_Clk;
	wb_rst_in  <= Bus2IP_Reset;
	wb_addr_in <= Bus2IP_Addr;
	wb_we_in   <= not Bus2IP_RNW;

	-- Wishbone state machine
	WB_STATE_MACHINE : process(slv_reg_write_sel, slv_reg_read_sel, st_current) is
		wb_stb_in <= '0';
		wb_cyc_in <= '0';
		wb_we_in  <= '0';
		readReq   <= '0';
		writeReq  <= '0';
		st_next <= st_current;
  
		case (st_current) is
			when StIdle =>
				readReq  <= or_reduce(slv_reg_read_sel);
				writeReq <= or_reduce(slv_reg_write_sel);
				if (readReq = '1') 
					st_next <= StRdReq;
				elsif (writeReq = '1')
					st_next <= StWrReq;
				else
					st_next <= st_idle;
				end if;
			when StRdReq =>
				wb_stb_in <= '1';		-- Read request to wb module
				wb_cyc_in <= '1';
				wb_we_in  <= '0';
				st_next <= st_ack;
			when StdWrReq =>
				wb_stb_in <= '1';		-- Write request to wb module
				wb_cyc_in <= '1';
				wb_we_in  <= '1';
				st_next <= st_ack;
			when StAck =>
				wb_stb_in <= '0';
				wb_cyc_in <= '0';
				wb_we_in  <= '0';
				readReq   <= '0';
				writeReq  <= '0';
				st_next <= StIdle;
		end case;
	end process WB_STATE_MACHINE;
  
	--    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
	--                     "1000"   C_BASEADDR + 0x0
	--                     "0100"   C_BASEADDR + 0x4
	--                     "0010"   C_BASEADDR + 0x8
	--                     "0001"   C_BASEADDR + 0xC
	slv_reg_write_sel <= Bus2IP_WrCE(0 to C_NUM_REG-1);
	slv_reg_read_sel  <= Bus2IP_RdCE(0 to C_NUM_REG-1);
	-- slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1) or Bus2IP_WrCE(2) or Bus2IP_WrCE(3);
	-- slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1) or Bus2IP_RdCE(2) or Bus2IP_RdCE(3);

	-- implement slave model software accessible register(s)
	SLAVE_REG_WRITE_PROC : process(writeReq) is
	begin
		if (writeReq = '1') then
			case slv_reg_write_sel is
				when "1000" =>
					for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
						if (Bus2IP_BE(byte_index) = '1' ) then
							wb_data_in(byte_index*8 to byte_index*8+7) <= 
								wb_encoder_in(byte_index*8 to byte_index*8+7);
							end if;
					end loop;
				when "0100" =>
					for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
						if (Bus2IP_BE(byte_index) = '1' ) then
							wb_data_in(byte_index*8 to byte_index*8+7) <= 
								Bus2IP_Data(byte_index*8 to byte_index*8+7);
						end if;
					end loop;
				when "0010" =>
					for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
						if (Bus2IP_BE(byte_index) = '1' ) then
							wb_data_in(byte_index*8 to byte_index*8+7) <= 
								Bus2IP_Data(byte_index*8 to byte_index*8+7);
						end if;
					end loop;
				when "0001" =>
					for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
						if (Bus2IP_BE(byte_index) = '1' ) then
							wb_data_in(byte_index*8 to byte_index*8+7) <= 
								Bus2IP_Data(byte_index*8 to byte_index*8+7);
						end if;
					end loop;
				when others => null;
			end case;
		end if;
	end process SLAVE_REG_WRITE_PROC;

	-- implement slave model software accessible register(s) read mux
	SLAVE_REG_READ_PROC : process(readReq) is
	begin
		case slv_reg_read_sel is
			when "1000" => slv_ip2bus_data <= wb_data_out;
			when "0100" => slv_ip2bus_data <= wb_data_out;
			when "0010" => slv_ip2bus_data <= wb_data_out;
			when "0001" => slv_ip2bus_data <= wb_data_out;
			when others => slv_ip2bus_data <= (others => '0');
		end case;
	end process SLAVE_REG_READ_PROC;

	WB_STATE_CURRENT: process(Bus2IP_Clk) is
	begin
		if(rising_edge(Bus2IP_Clk)) then
			if(Bus2IP_Reset = '1') then
				st_current <= StIdle;
			else
				st_current <= st_next;
			end if;
		end if;
	end process WB_STATE_CURRENT;

	-- WB route interrupt request
	IP2Bus_IntrEvent(0) <= wb_irq_out;
	-- WB route ack signals
	IP2Bus_RdAck <= wb_rdack;
	IP2Bus_WrAck <= wb_wrack;
	-- Always return no error?
	IP2Bus_Error <= '0';
	-- 
	IP2Bus_Data  <= slv_ip2bus_data when wb_rack = '1' else
					(others => '0');
end IMP;

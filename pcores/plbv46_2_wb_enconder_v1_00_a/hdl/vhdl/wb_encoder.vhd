----------------------------------------------------------------------------------
-- Company: National Polytechnic Institute
-- Engineer: Adrian Alonso <aalonso00@gmail.com>
-- 
-- Create Date:    17:48:30 04/07/2010 
-- Design Name: 
-- Module Name:    wb_encoder - behavioral 
-- Project Name:   wb_encoder
-- Target Devices: Virtex5
-- Tool versions: 
-- Description: Wishbone complaint wheel encoder module
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
--
-- wb_dreg  data register
-- wb_dreg (2) -> Push button
-- wb_dreg (1) -> Inc B
-- wb_dreg (0) -> Inc A
-- wb_creg  status/control register
-- wb_creg (2) -> Interrupt flag    *Not implemented
-- wb_creg (1) -> Interrupt ack
-- wb_creg (0) -> Interrupt enable

entity wb_encoder is
    generic (
        C_WB_DBUS_SIZE  : integer := 32;
        C_WB_ID         : integer := 2 -- Arbitrary selected device id
    );
    port ( 
        wb_rst_in   : in  std_logic;
        wb_clk_in   : in  std_logic;
        wb_stb_in   : in  std_logic;
        wb_cyc_in   : in  std_logic;
        wb_we_in    : in  std_logic;
        wb_addr_in  : in  std_logic_vector (0 to C_WB_DBUS_SIZE-1);
        wb_data_in   : in  std_logic_vector (0 to C_WB_DBUS_SIZE-1);
        wb_data_out   : out std_logic_vector (0 to C_WB_DBUS_SIZE-1);
        wb_irq_out  : out std_logic;
        wb_ack_out  : out std_logic);
end wb_encoder;

architecture behavioral of wb_encoder is
    -- helper function
    function or_reduce(l : std_logic_vector) return std_logic is
    variable v : std_logic := '0';
    begin
       for i in l'range loop v := v or l(i); end loop;
       return v;
    end;
    -- Signals
    signal r_ack: std_logic;
    signal w_ack: std_logic;
    signal wb_creg: std_logic_vector (0 to C_WB_DBUS_SIZE-1);
    signal wb_dreg: std_logic_vector (0 to C_WB_DBUS_SIZE-1);
    signal wb_ireg: std_logic_vector (0 to C_WB_DBUS_SIZE-1);

begin

    wb_ack_out <= r_ack or w_ack;
    -- read registers
    pread: process (wb_rst_in, wb_clk_in)
    begin
        if (wb_rst_in = '1') then
            r_ack <= '0';
            wb_data_out <= (others => '0');
        elsif (rising_edge(wb_clk_in)) then
            r_ack <= '0';
            -- reading registers
            if (wb_we_in = '0' and wb_stb_in = '1' 
                and wb_cyc_in = '1') then
                r_ack <= '1';
                if (wb_addr_in = X"0000_0000") then
                    wb_data_out <= wb_dreg;
                elsif (wb_addr_in = X"0000_0001") then
                    wb_data_out <= wb_creg;
                elsif (wb_addr_in = X"0000_0010") then
                    wb_data_out <= std_logic_vector(to_unsigned(C_WB_ID,C_WB_DBUS_SIZE));
                end if;
            -- writing registers
            else
                r_ack <= '0';
                wb_data_out <= (others => '0');
            end if;
        end if;
    end process pread;
    -- write registers
    pwrite: process (wb_rst_in, wb_clk_in)
    begin
        if (wb_rst_in = '1') then
            w_ack <= '0';
            wb_creg <= (others => '0');
        elsif (rising_edge(wb_clk_in)) then
            if (wb_we_in = '1' and wb_stb_in = '1'
                and wb_cyc_in = '1') then
                if (wb_addr_in = X"0000_0001") then
                    wb_creg <= wb_creg & wb_data_in;
                    w_ack <= '1';
                else
                    w_ack <= '0';
                end if;
            end if;
        end if;
    end process pwrite;
    -- monitor buttons
    pmon: process (wb_rst_in, wb_clk_in)
    begin
        if (wb_rst_in = '1') then
            wb_dreg <= (others => '0');
        elsif (rising_edge(wb_clk_in)) then
            wb_dreg <=  wb_data_in &  X"0000_0000";
        end if;
    end process pmon;
    -- generate interrupts
    pirq: process (wb_rst_in, wb_clk_in, wb_creg)
    begin
        if (wb_rst_in = '1' or wb_creg(1) = '1') then   -- Ack interrupt
            wb_irq_out <= '0';
            wb_ireg <= (others => '0');
        elsif (rising_edge(wb_clk_in)) then
            if (wb_creg(0) = '1') then                  -- Interrupts enabled
                if (wb_ireg /= wb_dreg) then
                    wb_irq_out <= '1';
                else
                    wb_irq_out <= '0';
                end if;                
                wb_ireg <= wb_dreg;
            end if;
        end if;
    end process pirq;
end behavioral;

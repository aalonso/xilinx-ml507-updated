/* wb_encoder.v wheel encoder module
 * Wishbone complaint
 *
 * Copyright (C) 2010 Adrian Alonso <aalonso00@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with self library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
*/

module wb_encoder
    (wb_clk_i,
     wb_rst_i,
     wb_we_i,
     wb_cyc_i,
     wb_stb_i,
     wb_ack_o,
     wb_data_i,
     wb_data_o,
     wb_addr_i,
     irq_o,
     enc_data
    );
    /* Parameters definition */
    parameter C_WB_DWIDTH   = 32;
    parameter C_WB_DATAREG  = {C_WB_DWIDTH {1'b0}};
    /* Port definitions */
    input wb_clk_i;
    input wb_rst_i;
    input wb_we_i;
    input wb_cyc_i;
    input wb_stb_i;
    output wb_ack_o;
    input  [0:C_WB_DWIDTH-1] wb_data_i;
    input  [0:C_WB_DWIDTH-1] wb_addr_i;
    output [0:C_WB_DWIDTH-1] wb_data_o;
    output irq_o;
    input  [0:2] enc_data;

    /* Internal module registers */
    reg [0:2] enc_reg;
    reg irq_o;
    reg wb_ack_o;
    reg [0:C_WB_DWIDTH-1] data_reg;
    reg [0:C_WB_DWIDTH-1] wb_data_o;

    /* Read registers */
    always @(posedge wb_clk_i)
    begin
        if(wb_rst_i == 1) begin
            wb_ack_o <= 1'b0;
            wb_data_o <= {C_WB_DWIDTH {1'b0}};
        end
        else begin
            if(wb_stb_i == 1 && wb_cyc_i ==1 &&
                wb_we_i == 0) begin
                if(C_WB_DATAREG == wb_addr_i)
                    wb_data_o <= data_reg;
                else
                    wb_data_o <= {C_WB_DWIDTH {1'b 0}};

                wb_ack_o <= 1'b1;
            end
            else begin
                wb_ack_o <= 1'b0;
                wb_data_o <= {C_WB_DWIDTH {1'b 0}};
            end
        end
    end

    /* Wheel encoder monitor signals */
    always @(posedge wb_clk_i)
    begin
        if(wb_rst_i == 1)
            data_reg <= {C_WB_DWIDTH {1'b 0}};
        else
            data_reg <= {enc_data, {C_WB_DWIDTH-3{1'b0}}};
    end

    /* Wheel encoder interrupt logic */
    always @(posedge wb_clk_i)
    begin
        if(wb_rst_i == 1) begin
            irq_o <= 1'b0;
            enc_reg <= 3'b000;
        end
        else begin
            if(enc_reg != enc_data)
                irq_o <= 1'b1;
            else
                irq_o <= 1'b0;
            enc_reg <= enc_data;
        end
    end
endmodule

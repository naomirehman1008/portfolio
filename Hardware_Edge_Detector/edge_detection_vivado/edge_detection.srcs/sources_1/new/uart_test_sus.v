`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 03:24:49 PM
// Design Name: 
// Module Name: uart_test_sus
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module uart_test_sus(
    input clkin, //100MHz clk
    input RsRx,
    input [15:0] sw,
    output RsTx,
    output [15:0] led
    );
    
    parameter CLOCK_FREQ=100000000;
    parameter SLOW_CLK_FREQ=10000000;
    
    wire clk;
    
    clk_XMHz #(.CLOCK_FREQ(CLOCK_FREQ), 
        .TARGET_FREQ(SLOW_CLK_FREQ)) slowClock (.clkin(clkin), 
        .clkout(clk));
    
    reg [7:0] out_data;
    wire [7:0] in_data;
    
    wire tx_DV;
    reg send_term_char;
    reg set_tx_DV;
    
    wire active;
    wire done;
    wire [2:0] state;
    uart_tx myTx(.i_Clock(clk), 
        .i_Tx_DV(tx_DV), 
        .i_Tx_Byte(out_data), 
        .o_Tx_Active(active),
        .o_Tx_Serial(RsTx),
        .o_Tx_Done(done)
        //.state(state)
        );
        
    wire rx_DV;
    reg [7:0] rx_data;
    uart_rx myRx(.i_Clock(clk),
        .i_Rx_Serial(RsRx),
        .o_Rx_DV(rx_DV),
        .o_Rx_Byte(in_data));
       
   //test receive
   reg [7:0] data;
   always @(*) begin
        if(rx_DV) begin
            data <= in_data;
        end
    end
    assign led[15:8] = data;
    
   //test transmit    
   /*
   initial begin
       out_data <= 8'd97;
   end
   assign tx_DV = sw[0];
   assign led[0] = active;
   assign led[1] = done;
   assign led[15:8] = out_data;
    */
endmodule

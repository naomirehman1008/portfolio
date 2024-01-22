`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/22/2023 03:52:11 PM
// Design Name: 
// Module Name: Syncs
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


module Syncs(
    input [14:0] H,
    input [14:0] V,
    input clk,
    output Hsync,
    output Vsync,
    output Active,
    output Frame
    );
    
    assign Active = (((15'b000001001111111) >= H) & ((15'b000000111011111) >= V));
    assign Hsync = ~(((15'b000001010001111) <= H) & ((15'b000001011101110) >= H));
    assign Vsync = ~(((15'b000000111101001) <= V) & ((15'b000000111101010) >= V));
    assign Frame = (H == 15'b000001010111100) & (V == 15'b000000111110100);
endmodule

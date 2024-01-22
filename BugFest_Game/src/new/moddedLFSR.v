`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/25/2023 12:33:37 PM
// Design Name: 
// Module Name: moddedLFSR
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


module moddedLFSR(
    input clk,
    input advance,
    input LD,
    input [7:0] Din,
    output [14:0] randOut
    );
    wire [7:0] Qout;
    
    LFSR randomNum(.clk(clk), .advance(advance), .LD(LD), .Din(Din), .Q(Qout));
    
    assign randOut = {{7{1'b0}},{1'b1}, Qout[6:2], {2{1'b0}}};
    
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/02/2023 09:18:33 PM
// Design Name: 
// Module Name: LFSR
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


module LFSR(
    input clk,
    input advance,
    input LD,
    input [7:0] Din,
    output [7:0] Q
    );
    wire [7:0] rnd;
    FDRE #(.INIT(1'b1) ) ff_0 (.C(clk), .R(1'b0), .CE(advance), .D((~LD & (rnd[0]^rnd[5]^rnd[6]^rnd[7])) | ((LD & Din[0]))), .Q(rnd[0]));
    FDRE #(.INIT(1'b0) ) ff[7:1] (.C({7{clk}}), .R({7{1'b0}}), .CE({7{advance}}), .D(({7{~LD}} & rnd[6:0]) | ({7{LD}} & Din[7:1])), .Q(rnd[7:1]));
    
    assign Q = rnd;
endmodule

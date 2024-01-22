`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/22/2023 02:42:06 PM
// Design Name: 
// Module Name: HV_coords
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


module HV_coords(
    input clk,
    output [14:0] H,
    output [14:0] V,
    output endFrame
    );
    wire [14:0] Hout, Vout;
    wire lastRow, lastCol;
    assign lastRow = &(Hout[14:0] ^ (15'b111110011100000));
    assign lastCol = &(Vout ^ ~(15'b000001000001100));
    counterUD15L Hcounter(.clk(clk), .Up(1'b1), .Dw(1'b0), .LD(lastRow), .Din({15{1'b0}}), .Q(Hout), .UTC(), .DTC());
    counterUD15L Vcounter(.clk(clk), .Up(lastRow), .Dw(1'b0), .LD(lastRow & lastCol), .Din({15{1'b0}}), .Q(Vout), .UTC(), .DTC());
    
    assign endFrame = lastRow & lastCol;
    assign H = Hout;
    assign V = Vout;
    
endmodule

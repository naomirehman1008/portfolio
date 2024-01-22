`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/24/2023 07:55:06 PM
// Design Name: 
// Module Name: counterUD15L
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


module counterUD15L(
    input clk,
    input Up,
    input Dw,
    input LD,
    input [14:0] Din,
    output [14:0] Q,
    output UTC,
    output DTC
    );
    
    wire utc_wire1, utc_wire2;
    wire dtc_wire1, dtc_wire2;
    wire utc_and1, utc_and2;
    wire dtc_and1, dtc_and2; //this naming scheme is horrible but here we are
    
    assign utc_and1 = utc_wire1 & Up;
    assign utc_and2 = utc_and1 & utc_wire2;
    //handle multiple inputs
    assign dtc_and1 = dtc_wire1 & Dw;
    assign dtc_and2 = dtc_and1 & dtc_wire2;
    
    countUD5L counter0(.clk(clk), .Up(Up), .Dw(Dw), .LD(LD), .Din(Din[4:0]), .Q(Q[4:0]), .UTC(utc_wire1), .DTC(dtc_wire1));
    countUD5L counter1(.clk(clk), .Up(utc_and1), .Dw(dtc_and1), .LD(LD), .Din(Din[9:5]), .Q(Q[9:5]), .UTC(utc_wire2), .DTC(dtc_wire2));
    countUD5L counter2(.clk(clk), .Up(utc_and2), .Dw(dtc_and2), .LD(LD), .Din(Din[14:10]), .Q(Q[14:10]), .UTC(UTC), .DTC(DTC)); //honestly none of this is tested
    
endmodule

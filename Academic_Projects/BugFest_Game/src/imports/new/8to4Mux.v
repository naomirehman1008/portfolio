`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/24/2023 03:43:09 PM
// Design Name: 
// Module Name: 10to5Mux
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


module Mux10to5(
    input [4:0] in1,
    input [4:0] in2,
    output [4:0] out,
    input sel
    );
    
    assign out[0] = (in1[0] & ~sel) | (in2[0] & sel);
    assign out[1] = (in1[1] & ~sel) | (in2[1] & sel);
    assign out[2] = (in1[2] & ~sel) | (in2[2] & sel);
    assign out[3] = (in1[3] & ~sel) | (in2[3] & sel);
    assign out[4] = (in1[4] & ~sel) | (in2[4] & sel); 
endmodule

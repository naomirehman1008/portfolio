`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2023 03:02:19 PM
// Design Name: 
// Module Name: bug_mod
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


module slug_mod(
    input clk,
    input UP,
    input DW,
    input leftEN,
    input Frame,
    input INIT,
    output [14:0] SlugH,
    output [14:0] SlugV
    );
    wire [14:0] vQ, hQ;
    //slug starts at V300
    counterUD15L vcounter(.clk(clk), .Up(UP & Frame), .Dw(DW & Frame), .LD(INIT), .Din(15'd95), .Q(vQ));
    counterUD15L hcounter(.clk(clk), .Up(1'b0), .Dw(leftEN & Frame), .LD(INIT), .Din(15'd140), .Q(hQ));
    
    assign SlugV = vQ << 1;
    assign SlugH = hQ;
    
endmodule

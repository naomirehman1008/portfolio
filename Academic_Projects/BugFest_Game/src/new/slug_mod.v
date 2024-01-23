`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2023 03:26:02 PM
// Design Name: 
// Module Name: slug_mod
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


module bug_mod(
    input DW,
    input clk,
    input Frame,
    input [7:0] Din,
    input INIT,
    input load,
    output [14:0] bugH,
    output [14:0] bugV
    );
    wire [7:0] randQ;
    wire [14:0] randIN0, randIN1;
    wire randSel;
    
    LFSR myRandom(.clk(clk), .advance(load), .LD(INIT), .Din(Din), .Q(randQ)); //new v value whenever the bug reaches the border
    
    assign randSel = randQ[0];
    //TODO: fill in these vectors
    assign randIN0 = {{7{1'b0}}, {1{1'b1}}, {2{1'b0}}, randQ[3:1], {2{1'b0}}};
    assign randIN1 = {{6{1'b0}}, {1{1'b1}}, {3{1'b0}}, randQ[3:1], {2{1'b0}}};
    
    wire [14:0] vQ, hQ;
    wire reset;

    counterUD15L hcounter(.clk(clk), .Up(1'b0), .Dw(DW & Frame), .LD(load | INIT | reset), .Din(15'd322), .Q(hQ));
    
    assign bugH = hQ << 1; //right side of bug
    assign bugV = (~{15{randSel}} & randIN0) | ({15{randSel}} & randIN1); //top of bug
    assign reset = &(~(hQ));
    


    
endmodule

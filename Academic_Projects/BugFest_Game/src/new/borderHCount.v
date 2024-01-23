`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/25/2023 12:53:52 PM
// Design Name: 
// Module Name: borderHCount
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


module borderHCount(
    input clk,
    input Frame,
    input init,
    input [7:0] Din,
    input platformDW,
    input [14:0] initBus,
    output [14:0] rand,
    output [14:0] Hupper
    );
    
    wire [14:0] randOut, Hcount;
    wire [14:0] max;
    wire reset;
    
    moddedLFSR randLen(.clk(clk), .advance(reset | init), .LD(init), .Din(Din), .randOut(randOut));
    counterUD15L counter(.clk(clk), .Up(1'b0), .Dw(platformDW & Frame), .LD(reset | init), .Din({15{reset}}&max | {15{init}}&initBus), .Q(Hcount));
    
    assign rand = randOut;
    assign Hupper = 15'd7 + Hcount;
    assign max = 15'd901;
    assign reset = &(~Hcount ^ 15'd1);
    
endmodule

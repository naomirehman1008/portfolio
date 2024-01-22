`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Naomi
// Engineer: Naomi
// 
// Create Date: 11/23/2023 03:42:09 PM
// Design Name: cpu-32-rv
// Module Name: general_register_file
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


module general_register_file #(
    parameter DATA_WIDTH=16,
    parameter NUM_REGISTERS=8,
    parameter REG_ADDR_BITS=3
)
(
    input clk,
    input wrreg,
    input [DATA_WIDTH-1:0] wrdata,
    input [REG_ADDR_BITS-1:0] wraddr,
    input [REG_ADDR_BITS-1:0] raddr1,
    input [REG_ADDR_BITS-1:0] raddr2,
    output [DATA_WIDTH-1:0] rdata1,
    output [DATA_WIDTH-1:0] rdata2
    );
    
    reg [DATA_WIDTH-1:0] regfile [DATA_WIDTH-1:0];
    
    always @(*) begin
        if(wrreg && wraddr != 0) begin
            regfile[wraddr] <= wrdata;
        end
        regfile[0] <= 0; //reg 0 always == 0
    end
    
    assign rdata1 = regfile[raddr1];
    assign rdata2 = regfile[raddr2];
    
endmodule

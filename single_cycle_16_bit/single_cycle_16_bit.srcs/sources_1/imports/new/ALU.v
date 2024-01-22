`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Naomi Rehman
// 
// Create Date: 11/24/2023 12:58:11 PM
// Design Name: 
// Module Name: ALU
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


module ALU #(
    parameter DATA_WIDTH=16,
    parameter NUM_REGISTERS=8,
    parameter REG_ADDR_BITS=3,
    parameter OP_WIDTH=3
)
(
    input clk,
    input [OP_WIDTH:0] ALUctl,
    input [DATA_WIDTH-1:0] A,
    input [DATA_WIDTH-1:0] B,
    input [DATA_WIDTH-1:0] PC,
    output reg [DATA_WIDTH-1:0] ALUOut,
    output zero
    );
    assign zero = (ALUOut == 0);
    always @(*) begin
        case (ALUctl)
            0: ALUOut <= A+B;   //ADD
            1: ALUOut <= A+B;   //ADDI
            2: ALUOut <= ~(A&B);   //NAND
            3: ALUOut <= A+B;   //LUI
            4: ALUOut <= A+B;   //SW
            5: ALUOut <= A+B;  //LW
            6: ALUOut <= A-B;  //BEQ
            7: ALUOut <= PC+1; //JALR
            default: ALUOut <= 0;
        endcase
   end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
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


module controlsig #(
    parameter OP_WIDTH=3
)
(
    input clk,
    input [OP_WIDTH-1:0] op,
    output reg Branch,
    output reg RegWrite,
    output reg ALUsrc,
    output reg MemWrite,
    output reg MemRead,
    output reg Mem2Reg,
    output reg U_Imdt
    );
    //this is really sus, def fix this later
    always @(*) begin
        case (op)
            3'b000: begin //ADD
                Branch <= 0;
                RegWrite <= 1;
                ALUsrc <= 0;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
            3'b001: begin //ADDI
                Branch <= 0;
                RegWrite <= 1;
                ALUsrc <= 1;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
            3'b010: begin //NAND
                Branch <= 0;
                RegWrite <= 1;
                ALUsrc <= 0;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
            3'b011: begin //LUI
                Branch <= 0;
                RegWrite <= 1;
                ALUsrc <= 1;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 1;
            end
            3'b100: begin //SW
                Branch <= 0;
                RegWrite <= 0;
                ALUsrc <= 1;
                MemRead <= 0;
                MemWrite <= 1;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
            3'b101: begin //LW
                Branch <= 0;
                RegWrite <= 1;
                ALUsrc <= 1;
                MemRead <= 1;
                MemWrite <= 0;
                Mem2Reg <= 1;
                U_Imdt <= 0;
            end
           3'b110: begin //BEQ
                Branch <= 1;
                RegWrite <= 0;
                ALUsrc <= 0;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
           3'b111: begin //JALR
                Branch <= 1;
                RegWrite <= 1;
                ALUsrc <= 0;
                MemRead <= 0;
                MemWrite <= 0;
                Mem2Reg <= 0;
                U_Imdt <= 0;
            end
        endcase
    end
endmodule
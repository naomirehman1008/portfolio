`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Naomi
// 
// Create Date: 01/22/2024 12:11:07 PM
// Design Name: 
// Module Name: memory
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


module memory #(
    parameter DATA_WIDTH=16,
    parameter ADDR_WIDTH=16
    )(
        input [ADDR_WIDTH-1:0] addr,
        input [DATA_WIDTH-1:0] write_data,
        input read_mem,
        input write_mem,
        output reg [DATA_WIDTH-1:0] read_data
    );
    
    reg [DATA_WIDTH-1:0] mem [ADDR_WIDTH-1:0];
    
    //initialize all memeory to 0
    initial begin
        for (int i=0; i < 2**ADDR_WIDTH; i++) begin
            mem[i] = {DATA_WIDTH{1'b0}};
        end
    end
    //can't read and write at the same time
    always @(*) begin
        if(read_mem) begin
            read_data <= mem[addr];
        end
        else if (write_mem) begin
            mem[addr] <= write_data;
        end
    end
endmodule

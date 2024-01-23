`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 03:40:34 PM
// Design Name: 
// Module Name: clk_5MHz
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


module clk_XMHz #(
    parameter CLOCK_FREQ=100000000,
    parameter TARGET_FREQ= 10000000
)(
    input clkin,
    output reg clkout
    );
    
    reg [4:0] counter; //5 bit counter
    initial begin
        clkout <= 0;
        counter <= 0; 
    end
    
    always @(posedge clkin) begin
        if(counter >= (CLOCK_FREQ / TARGET_FREQ - 1)) begin//count to 10
            clkout <= ~clkout;
            counter <= 0;
        end else begin
        counter = counter + 1;
        end
    end
endmodule

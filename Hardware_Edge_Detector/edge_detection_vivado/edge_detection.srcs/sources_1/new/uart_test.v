`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 02:48:49 PM
// Design Name: 
// Module Name: uart_test
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


module uart_test(
    input clkin,
    input rx,
    output tx
    );
    
    UART test_module(.clk(clkin), .rst(), .rx(), .tx(), .tx_data_in(), .rx_data_out());
endmodule

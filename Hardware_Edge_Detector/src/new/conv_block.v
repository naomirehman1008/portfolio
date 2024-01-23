`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/19/2024 09:05:04 PM
// Design Name: 
// Module Name: conv_block
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


module conv_block #(
    parameter IMAGE_WIDTH=14,
    parameter IMAGE_HIGHT=14,
    parameter DATA_WIDHT=8,
    parameter OUTPUT_WIDTH=IMAGE_WIDTH-3+1,
    parameter OUTPUT_HIGHT=IMAGE_HIGHT-3+1
)
//IDEA: add another input for the convolution filter type!
(
    input [DATA_WIDTH-1:0] input_image [IMAGE_WIDTH-1:0],
    output [DATA_WIDTH-1:0] output_image [I
    
    );
endmodule

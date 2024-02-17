`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: NAOMI :)
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
    parameter DATA_WIDTH=8,
    parameter OUTPUT_WIDTH=IMAGE_WIDTH-3+1,
    parameter OUTPUT_HIGHT=IMAGE_HIGHT-3+1
)
//IDEA: add another input for the convolution filter type!
    (
    input clk,
    input image_ready,
    input [DATA_WIDTH-1:0] i_input_image [IMAGE_WIDTH-1:0][IMAGE_HIGHT-1:0],
    output [DATA_WIDTH-1:0] output_image [OUTPUT_WIDTH-1:0][OUTPUT_HIGHT-1:0],
    output reg conv_done,
    output reg fucked_state
    );
    
    parameter s_CHILL = 2'b00;
    parameter s_CONVOLVE = 2'b01;
    parameter s_CONV_DONE = 2'b10;
    
    reg [DATA_WIDTH-1:0] input_image [IMAGE_WIDTH-1:0][IMAGE_HIGHT-1:0];
    reg [1:0] s_STATE;
        

    wire [2:0][2:0][DATA_WIDTH-1:0] input_kernel [OUTPUT_WIDTH-1:0][OUTPUT_HIGHT-1:0];
    //conv block done??
    genvar i;
    genvar j;
    genvar k;
    generate
        for (i = 0; i < OUTPUT_WIDTH; i = i+1) begin
            for (j=0; j < OUTPUT_HIGHT; j=j+1) begin
                //stupid verilog
                //wire input_kernel [2:0][(3*8-1):0];
                    assign input_kernel[i][j][0][0] = input_image[i][j];
                    assign input_kernel[i][j][0][1] = input_image[i][j+1];
                    assign input_kernel[i][j][0][2] = input_image[i][j+2];
                    assign input_kernel[i][j][1][0] = input_image[i+1][j];
                    assign input_kernel[i][j][1][1] = input_image[i+1][j+1];
                    assign input_kernel[i][j][1][2] = input_image[i+1][j+2];
                    assign input_kernel[i][j][2][0] = input_image[i+2][j];
                    assign input_kernel[i][j][2][1] = input_image[i+2][j+1];
                    assign input_kernel[i][j][2][2] = input_image[i+2][j+2];
                conv_unit #(DATA_WIDTH) (.input_data(input_kernel[i][j]), .output_data(output_image[i][j]));
            end
        end
    endgenerate
    
    initial begin
        s_STATE <= s_CHILL;
        conv_done <= 0;
        fucked_state <= 0;
        /*for(int x=0; x<OUTPUT_WIDTH;x++) begin
            for(int y=0; y<OUTPUT_HIGHT; y++) begin
                output_image[x][y]
            end
        end*/
    end
    always @(posedge clk) begin
        case(s_STATE)
            s_CHILL: begin
                fucked_state <= 0;
                conv_done <= 0;
                if(image_ready) begin
                    s_STATE <= s_CONVOLVE;
                    input_image <= i_input_image;
                end else begin
                    s_STATE <= s_CHILL;
                end 
            end
            s_CONVOLVE: begin
                fucked_state <= 0;
                s_STATE <= s_CONV_DONE;
            end
            s_CONV_DONE: begin
                fucked_state <= 0;
                conv_done <= 1; 
                s_STATE <= s_CHILL;
            end  
            default: begin
                fucked_state <= 1;
            end
                      
        endcase
    end
endmodule


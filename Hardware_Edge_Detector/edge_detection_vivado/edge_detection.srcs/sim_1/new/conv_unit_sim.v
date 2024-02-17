`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/19/2024 08:49:47 PM
// Design Name: 
// Module Name: conv_unit_sim
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

module conv_unit_sim();
    parameter DATA_WIDTH=8;
    reg clk;
    reg [2:0][2:0][DATA_WIDTH-1:0] input_data;
    reg [7:0] output_data;
    
    conv_unit #(DATA_WIDTH)unit
    (
    .clk(clk),
    .input_data(input_data),
    .output_data(output_data)
    );
    
    
    
  // Clock generation
  always #5 clk = ~clk;

  // Initial block
  initial begin
    // Initialize signals
    clk = 0;

    // Apply reset for a few clock cycles
    #10;

    // Generate test pattern
    // Initialize input_image with one half 0x00 and one half 0x7F
    for (int i = 0; i < 2; i = i + 1) begin
      for (int j = 0; j < 2; j = j + 1) begin
        i_input_image[i][j] = 8'h00;
      end
    end

    for (int i = 0; i < IMAGE_WIDTH; i = i + 1) begin
      for (int j = IMAGE_HEIGHT/2; j < IMAGE_HEIGHT; j = j + 1) begin
        input_data[i][j] = 8'h7F;
      end
    end

    // Start the simulation
    image_ready = 1;

    // Finish simulation after some time
    #100;
    $finish;
  end


endmodule

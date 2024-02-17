`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/21/2024 08:31:02 PM
// Design Name: 
// Module Name: conv_unit_simu
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
    for (int i = 0; i < 3; i = i + 1) begin
      for (int j = 0; j < 3; j = j + 1) begin
        input_data[i][j] = i;
      end
    end
    input_data[0][0] = 8'b0;
    input_data[0][1] = 8'b0;
    input_data[0][2] = 8'b1;
    input_data[1][0] = 8'b1;
    input_data[1][1] = 8'b1;
    input_data[1][2] = 8'b0;
    input_data[2][0] = 8'b10;
    input_data[2][1] = 8'b10;
    input_data[2][2] = 8'b10;
    

    // Start the simulation
    

    // Finish simulation after some time
    #100;
    $finish;
  end


endmodule


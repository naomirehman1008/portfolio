`timescale 1ns / 1ps

module tb_image_array_to_uart;

  // Parameters
  parameter IMAGE_WIDTH = 14;
  parameter IMAGE_HEIGHT = 14;
  parameter OUTPUT_WIDTH = IMAGE_WIDTH - 3 + 1;
  parameter OUTPUT_HEIGHT = IMAGE_HEIGHT - 3 + 1;
  parameter DATA_WIDTH = 8;

  // Inputs
  reg clk;
  reg [7:0] output_image [OUTPUT_WIDTH-1:0][OUTPUT_HEIGHT-1:0];
  reg conv_ready;
  reg tx_active;
  reg tx_done;

  // Outputs
  wire [7:0] tx_data;
  wire write_uart;
  wire frame_sent;
  wire active;
  wire fucked_state;

  // Instantiate the module
  image_array_to_uart #(
    .IMAGE_WIDTH(IMAGE_WIDTH),
    .IMAGE_HEIGHT(IMAGE_HEIGHT),
    .OUTPUT_WIDTH(OUTPUT_WIDTH),
    .OUTPUT_HEIGHT(OUTPUT_HEIGHT),
    .DATA_WIDTH(DATA_WIDTH)
  ) uut (
    .clk(clk),
    .output_image(output_image),
    .conv_ready(conv_ready),
    .tx_active(tx_active),
    .tx_done(tx_done),
    .tx_data(tx_data),
    .write_uart(write_uart),
    .frame_sent(frame_sent),
    .active(active),
    .fucked_state(fucked_state)
  );

  // Clock generation
  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end

  // Test stimulus
 // Test Scenario 1: Basic functionality test
initial begin
  // Initialize inputs
  clk = 0;
  conv_ready = 0;
  tx_active = 0;
  tx_done = 0;

  // Apply stimulus
  // Set conv_ready to 1 to start the process
  conv_ready = 1;

  // Wait for a few clock cycles
  #10;

  // Monitor outputs
  // Check if frame_sent is asserted after processing
  if (frame_sent === 1) 
    $display("Test Scenario 1 Passed: Basic functionality test");
  else
    $display("Test Scenario 1 Failed: Basic functionality test");

  // Add additional checks if needed

  // Terminate simulation
  $finish;
end

// Test Scenario 2: Test case with specific input data
initial begin
  // Initialize inputs
  clk = 0;
  conv_ready = 0;
  tx_active = 0;
  tx_done = 0;

  // Apply stimulus
  // Set conv_ready to 1 to start the process
  conv_ready = 1;

  // Provide specific input data in output_image array
  
  // Wait for a sufficient number of clock cycles

  // Monitor outputs
  // Check the expected values of tx_data, write_uart, frame_sent, etc.

  // Add additional checks if needed

  // Terminate simulation
  $finish;
end

// Add more test scenarios as needed


endmodule

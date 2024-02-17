`timescale 1ns / 1ps

module tb_uart_to_image_array;

  // Parameters
  parameter IMAGE_WIDTH = 14;
  parameter IMAGE_HEIGHT = 14;
  parameter DATA_WIDTH = 8;
  parameter START_CHAR = 8'd90; // fix these
  parameter STOP_CHAR = 8'd90;

  // Inputs
  reg clk;
  reg rx_empty;
  reg [7:0] rx_data;

  // Outputs
  wire [DATA_WIDTH-1:0] input_image [IMAGE_WIDTH-1:0][IMAGE_HEIGHT-1:0];
  wire image_ready, read_uart, active, mangled_packet;
  wire [2:0] state;
  wire fucked_state;

  // Instantiate the uart_to_image_array module
  uart_to_image_array #(
    .IMAGE_WIDTH(IMAGE_WIDTH),
    .IMAGE_HEIGHT(IMAGE_HEIGHT),
    .DATA_WIDTH(DATA_WIDTH),
    .START_CHAR(START_CHAR),
    .STOP_CHAR(STOP_CHAR)
  ) uut (
    .rx_empty(rx_empty),
    .clk(clk),
    .rx_data(rx_data),
    .input_image(input_image),
    .image_ready(image_ready),
    .read_uart(read_uart),
    .active(active),
    .mangled_packet(mangled_packet),
    .state(state),
    .fucked_state(fucked_state)
  );

  // Clock generation
  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end

  // Test scenario
  initial begin
    // Initialize inputs
    rx_empty = 1;
    rx_data = 8'h00;

    // Apply reset
    #100;
    rx_empty = 0;

    // Send start character
    #10;
    rx_data = START_CHAR;

    // Send image data
    repeat (IMAGE_WIDTH * IMAGE_HEIGHT) begin
      #86810;
      rx_data = $random;
      #5;
      rx_empty = 1;
      // Simulate delay between characters
      #15;
      rx_empty = 0;
    end

    // Send stop character
    #10;
    rx_data = STOP_CHAR;

    // Wait for image_ready signal
    #1000;  // Adjust based on the expected processing time
    if (image_ready) $stop;

    // Print final state
    $display("Simulation finished. State: %b", state);
  end

endmodule

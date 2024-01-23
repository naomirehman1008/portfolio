`timescale 1ns / 1ps

module conv_block_sim();

  // Parameters
  parameter IMAGE_WIDTH = 14;
  parameter IMAGE_HEIGHT = 14;
  parameter DATA_WIDTH = 8;
  parameter OUTPUT_WIDTH = IMAGE_WIDTH - 3 + 1;
  parameter OUTPUT_HEIGHT = IMAGE_HEIGHT - 3 + 1;

  // Signals
  reg clk;
  reg image_ready;
  reg [DATA_WIDTH-1:0] i_input_image [IMAGE_WIDTH-1:0][IMAGE_HEIGHT-1:0];
  reg [DATA_WIDTH-1:0] output_image [OUTPUT_WIDTH-1:0][OUTPUT_HEIGHT-1:0];
  reg conv_done;
  reg fucked_state;

  // Instantiate conv_block module
  conv_block #(IMAGE_WIDTH, IMAGE_HEIGHT, DATA_WIDTH, OUTPUT_WIDTH, OUTPUT_HEIGHT)
    uut (
      .clk(clk),
      .image_ready(image_ready),
      .i_input_image(i_input_image),
      .output_image(output_image),
      .conv_done(conv_done),
      .fucked_state(fucked_state)
    );

  // Clock generation
  always #5 clk = ~clk;

  // Initial block
  initial begin
    // Initialize signals
    clk = 0;
    image_ready = 0;

    // Apply reset for a few clock cycles
    #10;

    // Generate test pattern
    // Initialize input_image with one half 0x00 and one half 0x7F
    for (int i = 0; i < IMAGE_WIDTH; i = i + 1) begin
      for (int j = 0; j < IMAGE_HEIGHT/2; j = j + 1) begin
        i_input_image[i][j] = 8'h00;
      end
    end

    for (int i = 0; i < IMAGE_WIDTH; i = i + 1) begin
      for (int j = IMAGE_HEIGHT/2; j < IMAGE_HEIGHT; j = j + 1) begin
        i_input_image[i][j] = 8'h7F;
      end
    end

    // Start the simulation
    image_ready = 1;

    // Finish simulation after some time
    #100;
    $finish;
  end

endmodule

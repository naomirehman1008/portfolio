`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 09:45:18 PM
// Design Name: 
// Module Name: convolution_top
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

//potential issue: multiple images sent before the first is sent back

module convolution_top(
    input clkin, //100MHz clk
    input btnR,
    input RsRx,
    input btnL,
    //input [15:0] sw,
    output RsTx,
    output [15:0] led
    );
    //params
    parameter CLOCK_FREQ=100000000;
    parameter SLOW_CLK_FREQ = 10000000;
    parameter BAUD_RATE = 115200;
    parameter CLKS_PER_BIT = $floor(SLOW_CLK_FREQ/BAUD_RATE);
    parameter IMAGE_WIDTH=8;
    parameter IMAGE_HIGHT=8;
    parameter OUTPUT_WIDTH=IMAGE_WIDTH -3 + 1;
    parameter OUTPUT_HIGHT=IMAGE_HIGHT -3 + 1;
    parameter DATA_WIDTH=8;
    parameter START_CHAR=8'd90;
    //parameter START_CHAR=8'd130;
    parameter STOP_CHAR=8'd90;
    //parameter STOP_CHAR=
        
    //wires 
    wire clk;
    
    //uart wires
    wire read_uart;
    wire write_uart;
    wire [7:0] tx_data;
    wire [7:0] rx_data;
    wire rx_full;
    wire rx_empty;
    
    //conv_block vars
    wire [DATA_WIDTH-1:0] conv_i_input_image[IMAGE_WIDTH-1:0][IMAGE_HIGHT-1:0];
    wire [DATA_WIDTH-1:0] conv_output_image[OUTPUT_WIDTH-1:0][OUTPUT_HIGHT-1:0];
    wire conv_done;
    wire conv_lost_state;
    //conv_block assigns
    
    //uart to image vars
    wire u2i_image_ready;
    wire u2i_active;
    wire u2i_mangled_packet;
    wire u2i_lost_state;
    //uart to image assigns
    
    //image to uart vars
    wire i2u_frame_sent;
    wire i2u_active;
    wire i2u_lost_state;
    //image to uart assigns
    
    //probably don't need this but I'm paranoid
    //clk_XMHz #(.CLOCK_FREQ(CLOCK_FREQ), 
    //    .TARGET_FREQ(SLOW_CLK_FREQ)) slowClock (.clkin(clkin), 
    //    .clkout(clk));
    assign clk = clkin;    

    
    
    uart_top UART_UNIT
        (
            .clk_100MHz(clkin),
            .reset(btnR),
            .read_uart(read_uart),
            .write_uart(write_uart),
            .rx(RsRx),
            .write_data(tx_data),
            .rx_full(rx_full),
            .rx_empty(rx_empty),
            .read_data(rx_data),
            .tx(RsTx)
        );
  
    
    conv_block #(.IMAGE_WIDTH(IMAGE_WIDTH),
        .IMAGE_HIGHT(IMAGE_HIGHT),
        .DATA_WIDTH(DATA_WIDTH)) myConvBlock (.clk(clk),
        .image_ready(u2i_image_ready),
        .i_input_image(conv_i_input_image),
        .output_image(conv_output_image),
        .conv_done(conv_done),
        .lost_state(conv_lost_state));
    
    wire [2:0] u_state;
    wire [2:0] i_state;
    
    uart_to_image_array #(.IMAGE_WIDTH(IMAGE_WIDTH),
        .IMAGE_HIGHT(IMAGE_HIGHT),
        .DATA_WIDTH(DATA_WIDTH),
        .START_CHAR(START_CHAR),
        .STOP_CHAR(STOP_CHAR)) uart2img (.rx_empty(rx_empty),
        .clk(clk),
        .rx_data(rx_data),
        .input_image(conv_i_input_image),
        .image_ready(u2i_image_ready),
        .read_uart(read_uart),
        .active(u2i_active),
        .mangled_packet(u2i_mangled_packet),
        .state(u_state),
        .lost_state(u2i_lost_state));
     
    image_array_to_uart #(.IMAGE_WIDTH(IMAGE_WIDTH),
        .IMAGE_HIGHT(IMAGE_HIGHT),
        .DATA_WIDTH(DATA_WIDTH)) img2uart (.clk(clk),
        .output_image(conv_output_image),
        .conv_ready(conv_done),
        //.tx_active(Tx_Active),
        //.tx_done(Tx_Done),
        .tx_data(tx_data),
        .write_uart(write_uart),
        .frame_sent(i2u_frame_sent),
        .active(i2u_active),
        .state(i_state),
        .lost_state(i2u_lost_state));
    
        
    //Testing stuff
    //assign led[0] = conv_fucked_state;
    assign led[0] = u2i_image_ready;
    assign led[1] = conv_done;
    assign led[2] = i2u_frame_sent;
    //assign led[3] = i2u_fucked_state;
    assign led[4] = rx_empty;
    assign led[5] = rx_full;
    assign led[12:10] = i_state;
    assign led[15:13] = u_state;
endmodule

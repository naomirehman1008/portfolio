`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 03:00:58 PM
// Design Name: 
// Module Name: uart_to_image_array
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


module uart_to_image_array #(
    parameter IMAGE_WIDTH=14,
    parameter IMAGE_HIGHT=14,
    parameter DATA_WIDTH=8,
    parameter START_CHAR=8'd90, //fix these
    parameter STOP_CHAR=8'd90
)
(
    input rx_empty,
    input clk,
    input [DATA_WIDTH-1:0] rx_data,
    output reg [DATA_WIDTH-1:0] input_image [IMAGE_WIDTH-1:0][IMAGE_HIGHT-1:0],
    output reg image_ready,
    output reg read_uart,
    output reg active,
    output reg mangled_packet,
    output [2:0] state,
    output reg lost_state
    );
    
    parameter s_CHILL = 3'b000;
    parameter s_WAIT_FOR_DATA = 3'b001;
    parameter s_WAIT_FOR_STOP_CHAR = 3'b010;
    parameter s_READ_DATA = 3'b011;
    parameter s_DATA_READY = 3'b100;
    parameter s_STOP_CHAR = 3'b101;
    
    
    reg [2:0] s_STATE;
    reg start_char_found;
    
    assign state = s_STATE;
    
    reg [$clog2(IMAGE_WIDTH)-1:0] w_counter;
    reg [$clog2(IMAGE_HIGHT)-1:0] h_counter;
    
    // TODO add a timeout counter to recover state
    initial begin
        image_ready <= 0;
        s_STATE <= s_CHILL;
        w_counter <= 0;
        h_counter <= 0;
        active <= 0;
        mangled_packet <= 0;
        lost_state <= 0;
        start_char_found <= 0;
    end
    always @(posedge clk)
        case (s_STATE)
            s_CHILL: begin
                lost_state <= 0;
                start_char_found <= 0;
                image_ready <= 0;
                if(!(rx_empty)) begin
                    read_uart <= 1;
                    active <= 1;
                    s_STATE <= s_READ_DATA;
                end
                else begin
                    read_uart <= 0;
                    s_STATE <= s_CHILL;
                    active <= 0;
                end      
            end
            s_WAIT_FOR_DATA: begin
                if(rx_empty) begin
                    s_STATE <= s_WAIT_FOR_DATA;
                end else begin
                    read_uart <= 1;
                    s_STATE <= s_READ_DATA;
                end
            end
            s_READ_DATA: begin
                lost_state<= 0;
                mangled_packet <= 0;
                if(!(start_char_found)) begin
                    if(rx_data == START_CHAR) begin
                        read_uart <= 0;
                        start_char_found <= 1;
                        s_STATE <= s_WAIT_FOR_DATA;
                    end else begin
                        read_uart <= 0;
                        active = 0;
                        s_STATE <=s_CHILL;
                    end
                end
                else begin;
                    input_image[w_counter][h_counter] <= rx_data;
                    if (w_counter == IMAGE_WIDTH-1 && h_counter == IMAGE_HIGHT-1) begin
                        read_uart <= 0;
                        w_counter <= 0;
                        h_counter <= 0;
                        s_STATE <= s_WAIT_FOR_STOP_CHAR;
                    end else if(w_counter == IMAGE_WIDTH-1) begin
                        read_uart <= 0;
                        w_counter <= 0;
                        h_counter <= h_counter + 1;
                        s_STATE <= s_WAIT_FOR_DATA;
                    end else begin
                        read_uart <= 0;
                        w_counter <= w_counter + 1;
                        s_STATE <= s_WAIT_FOR_DATA;
                    end
                end
            end
            s_WAIT_FOR_STOP_CHAR: begin
                lost_state <= 0;
                if(rx_empty) begin
                    s_STATE <= s_WAIT_FOR_STOP_CHAR;
                end else begin
                    read_uart <= 1;
                    s_STATE <= s_STOP_CHAR;
                end
            end
            s_STOP_CHAR: begin
                lost_state <= 0;
                if(rx_data == STOP_CHAR) begin
                    read_uart <= 0;
                    s_STATE <= s_DATA_READY;
                end
                else begin
                    read_uart <= 0;
                    mangled_packet <= 1;
                    s_STATE <= s_CHILL;
                    active <= 0;
                end
            end
            s_DATA_READY: begin
                read_uart <= 0;
                lost_state<= 0;
                active <= 0;
                mangled_packet <= 0;
                image_ready <= 1;
                s_STATE <= s_CHILL;
            end
            default: begin
                lost_state<= 1;
               // s_STATE <= s_CHILL;
            end
        endcase              
endmodule

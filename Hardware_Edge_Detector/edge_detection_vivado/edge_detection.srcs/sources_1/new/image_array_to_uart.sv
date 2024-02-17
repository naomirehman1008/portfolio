`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 08:17:19 PM
// Design Name: 
// Module Name: image_array_to_uart
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


module image_array_to_uart #(
    parameter IMAGE_WIDTH=14,
    parameter IMAGE_HIGHT=14,
    parameter OUTPUT_WIDTH=IMAGE_WIDTH-3+1,
    parameter OUTPUT_HIGHT=IMAGE_HIGHT-3+1,
    parameter DATA_WIDTH=8,
    parameter BR_LIMIT=104,
    parameter BR_BITS=7
)( 
    input clk,
    input [DATA_WIDTH-1:0] output_image [OUTPUT_WIDTH-1:0][OUTPUT_HIGHT-1:0],
    input conv_ready,
    input tx_active,
    input tx_done,
    output reg [7:0] tx_data,
    output reg write_uart,
    output reg frame_sent,
    output reg active,
    output [2:0] state,
    output reg lost_state

    );
    
    parameter s_CHILL = 3'b000;
    parameter s_LOAD_BYTE = 3'b001;
    parameter s_TX_VD = 3'b010;
    parameter s_INC_COUNTERS = 3'b011;
    parameter s_IMAGE_SENT = 3'b111;
    
    wire tick;
    reg [2:0] s_STATE;
        baud_rate_generator 
        #(
            .M(BR_LIMIT), 
            .N(BR_BITS)
         ) 
        BAUD_RATE_GEN   
        (
            .clk_100MHz(clk), 
            .reset(),
            .tick(tick)
         );
    assign state = s_STATE;
    
    reg set_tx_DV;
    
    reg [$clog2(IMAGE_WIDTH)-1:0] w_counter;
    reg [$clog2(IMAGE_HIGHT)-1:0] h_counter;
    
    initial begin
        s_STATE <= s_CHILL;
        set_tx_DV <= 0;
        write_uart <= 0;
        w_counter <= 0;
        h_counter <= 0;
        frame_sent <= 0;
        active <= 0;
        lost_state <= 0;
    end
    
    always @(posedge clk) begin
        case (s_STATE)
            s_CHILL: begin
                frame_sent <= 0;
                active <= 0;
                lost_state <= 0;
                w_counter <= 0;
                h_counter <= 0;
                if(conv_ready) begin
                    s_STATE <= s_LOAD_BYTE;
                    active <= 1;
                end else begin
                    s_STATE <= s_CHILL;
                end
            end
            s_LOAD_BYTE: begin
                lost_state <= 0;
                tx_data <= output_image[w_counter][h_counter];
                s_STATE <= s_TX_VD;
                set_tx_DV <= 1;
            end
            s_TX_VD: begin
                lost_state <= 0;
                if(set_tx_DV) begin
                    set_tx_DV <= 0;
                    write_uart <= 1;
                end
                if(!tick) begin
                    write_uart <= 0;
                    s_STATE <= s_TX_VD;
                end else begin
                    write_uart <= 1;
                    s_STATE = s_INC_COUNTERS;
                end
            end
            s_INC_COUNTERS: begin
                lost_state <= 0;
                //all bytes sent
                if (w_counter == OUTPUT_WIDTH-1 && h_counter == OUTPUT_HIGHT-1) begin
                    w_counter <= 0;
                    h_counter <= 0;
                    s_STATE <= s_IMAGE_SENT;
                // move down one row
                end else if(w_counter == OUTPUT_WIDTH-1) begin
                    w_counter <= 0;
                    h_counter <= h_counter + 1;
                    s_STATE <= s_LOAD_BYTE;
                //move right one row
                end else begin
                    w_counter <= w_counter + 1;
                    s_STATE <= s_LOAD_BYTE;
                end
            end
            s_IMAGE_SENT: begin
                lost_state <= 0;
                frame_sent <= 1;
                s_STATE <= s_CHILL;
            end
            default: begin
                lost_state <= 1;
            end
        endcase
    end
endmodule

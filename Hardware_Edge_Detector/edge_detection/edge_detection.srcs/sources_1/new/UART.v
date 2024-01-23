`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 03:22:56 PM
// Design Name: 
// Module Name: UART
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


module UART (
    input wire clk,
    input wire rst,
    input wire rx,
    input wire transmit, //tells uart to send data
    input wire [7:0] tx_data_in,
    output wire [7:0] rx_data_out,
    output wire tx,
    output reg transmission_complete,
    output wire data_ready
);

parameter BAUD_RATE = 9600;
parameter CLOCK_FREQ = 100_000_000; // Assuming a 100 MHz clock

reg [9:0] tx_count;
reg [9:0] rx_count;
reg [3:0] tx_bit_count;
reg [3:0] rx_bit_count;
reg [7:0] tx_shift_reg;
reg [7:0] rx_shift_reg;
reg [15:0] tx_baud_counter;
reg [15:0] rx_baud_counter;
reg tx_reg;
reg rx_reg;
reg data_ready_internal;

always @(posedge clk or posedge rst) begin
    //reset
    if (rst) begin
        tx_count <= 0;
        rx_count <= 0;
        tx_bit_count <= 0;
        rx_bit_count <= 0;
        tx_baud_counter <= 0;
        rx_baud_counter <= 0;
        tx_shift_reg <= 8'b0;
        rx_shift_reg <= 8'b0;
        tx_reg <= 1; // TX normally high
        rx_reg <= 0; // Receive data
        data_ready_internal <= 0;
        transmission_complete <= 1;
    //transmit
    end else begin
        // Baud rate generator for TX
        if (tx_baud_counter == CLOCK_FREQ / (BAUD_RATE * 16) - 1) begin
            tx_baud_counter <= 0;
        end else begin
            tx_baud_counter <= tx_baud_counter + 1;
        end

        // Transmit
        if (transmit && tx_baud_counter == 0 && tx_count < 10) begin
            if (tx_count == 0) begin
                transmission_complete <= 0;
                tx_shift_reg <= tx_data_in;
                tx_reg <= 0; // Start bit
            end 
            else if (tx_count == 9) begin
                tx_reg <= 1; // Stop bit
                tx_count <= 0;
                transmission_complete <= 1;
            end
            else begin
                tx_reg <= tx_shift_reg[tx_bit_count];
                tx_bit_count <= tx_bit_count + 1;
            end
            tx_count <= tx_count + 1;
        end

        // Baud rate generator for RX
        if (rx_baud_counter == CLOCK_FREQ / (BAUD_RATE * 16) - 1) begin
            rx_baud_counter <= 0;
        end else begin
            rx_baud_counter <= rx_baud_counter + 1;
        end

        // Receive
        if (rx_baud_counter == 0 && rx_count < 10) begin
            if (rx_count == 0) begin
                rx_reg <= rx;
            end else if (rx_count == 8) begin
                rx_shift_reg <= rx_shift_reg << 1 | rx_reg;
                rx_count <= 0;
                data_ready_internal <= 1; // Data ready when all bits are received
            end else begin
                rx_count <= rx_count + 1;
            end
        end
    end
end

always @(posedge clk or posedge rst) begin
    if (rst) begin
        tx_shift_reg <= 8'b0;
    end 
end

assign rx_data_out = rx_shift_reg;
assign tx = tx_reg;
assign data_ready = data_ready_internal;

endmodule


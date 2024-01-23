`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Reference Book: FPGA Prototyping By Verilog Examples Xilinx Spartan-3 Version
// Authored by: Dr. Pong P. Chu
// Published by: Wiley
//
// Adapted for the Basys 3 Artix-7 FPGA by David J. Marion
//
// UART System Verification Circuit
//
// Comments:
// - Many of the variable names have been changed for clarity
//////////////////////////////////////////////////////////////////////////////////

module uart_test(
    input clkin,       // basys 3 FPGA clock signal
    input btnR,            // reset 
    input RsRx,               // USB-RS232 Rx
    input btnL,              // btnL (read and write FIFO operation)
    output RsTx,              // USB-RS232 Tx
    //output [3:0] an,        // 7 segment display digits
    //output [0:6] seg,       // 7 segment display segments
    output [15:0] led        // data byte display
    );
    
    // Connection Signals
    wire rx_full, rx_empty, btn_tick;
    wire [7:0] rec_data, rec_data1;
    
    // Complete UART Core
    uart_top UART_UNIT
        (
            .clk_100MHz(clkin),
            .reset(btnR),
            .read_uart(btn_tick),
            .write_uart(btn_tick),
            .rx(RsRx),
            .write_data(rec_data1),
            .rx_full(rx_full),
            .rx_empty(rx_empty),
            .read_data(rec_data),
            .tx(RsTx)
        );
    
    // Button Debouncer
    debounce_explicit BUTTON_DEBOUNCER
        (
            .clk_100MHz(clkin),
            .reset(btnR),
            .btn(btnL),         
            .db_level(),  
            .db_tick(btn_tick)
        );
    
    // Signal Logic    
    assign rec_data1 = rec_data + 1;    // add 1 to ascii value of received data (to transmit)
    
    // Output Logic
    assign led[15] = rx_full;
    assign led[7] = rx_empty;
    assign led[14:8] = rec_data[6:0];              // data byte received displayed on LEDs
    assign led[6:0] = rec_data1[6:0];
endmodule
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/24/2023 12:15:05 PM
// Design Name: 
// Module Name: countUD5L
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


module countUD5L(
    input clk,
    input Up,
    input Dw,
    input LD,
    input [4:0] Din,
    output [4:0] Q,
    output UTC,
    output DTC
    );
    wire [4:0] q_wire;
    wire [4:0] d_wire;
    wire [4:0] and_wire;
    wire [4:0] mux_wire_q;
    wire [4:0] mux_wire_and;
    wire inc;
    wire enable;
    assign enable = (Up | Dw | LD);
    wire count_enable;
    assign count_enable = (Up | Dw) & ~LD;
    assign inc = ~Up & Dw; //0 is up, //1 is down
    
    Mux10to5 myMux_Q(.in1(q_wire[4:0]), .in2(~q_wire[4:0]), .sel(inc), .out(mux_wire_q[4:0]));
    Mux10to5 myMux_AND(.in1(and_wire[4:0]), .in2(~and_wire[4:0]), .sel(inc), .out(mux_wire_and[4:0]));
    
    FDRE #(.INIT(1'b0) ) ff_0 (.C(clk), .R(1'b0), .CE(enable), .D(d_wire[0]), .Q(q_wire[0]));
    FDRE #(.INIT(1'b0) ) ff_1 (.C(clk), .R(1'b0), .CE(enable), .D(d_wire[1]), .Q(q_wire[1]));
    FDRE #(.INIT(1'b0) ) ff_2 (.C(clk), .R(1'b0), .CE(enable), .D(d_wire[2]), .Q(q_wire[2]));    
    FDRE #(.INIT(1'b0) ) ff_3 (.C(clk), .R(1'b0), .CE(enable), .D(d_wire[3]), .Q(q_wire[3]));
    FDRE #(.INIT(1'b0) ) ff_4 (.C(clk), .R(1'b0), .CE(enable), .D(d_wire[4]), .Q(q_wire[4]));
    
    assign and_wire[0] = count_enable;
    assign and_wire[1] = and_wire[0] & mux_wire_q[0];
    assign and_wire[2] = and_wire[1] & mux_wire_q[1];
    assign and_wire[3] = and_wire[2] & mux_wire_q[2];
    assign and_wire[4] = and_wire[3] & mux_wire_q[3];
    
    assign d_wire[0] = ((mux_wire_and[0] ^ mux_wire_q[0]) & count_enable) | (LD & Din[0]);
    assign d_wire[1] = ((mux_wire_and[1] ^ mux_wire_q[1]) & count_enable) | (LD & Din[1]);
    assign d_wire[2] = ((mux_wire_and[2] ^ mux_wire_q[2]) & count_enable) | (LD & Din[2]);
    assign d_wire[3] = ((mux_wire_and[3] ^ mux_wire_q[3]) & count_enable) | (LD & Din[3]);
    assign d_wire[4] = ((mux_wire_and[4] ^ mux_wire_q[4]) & count_enable) | (LD & Din[4]);
    
    assign Q[4:0] = q_wire[4:0];
    assign UTC = &q_wire;
    assign DTC = ~(|q_wire);
    
endmodule

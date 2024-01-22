`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/24/2023 11:26:08 AM
// Design Name: 
// Module Name: Ring Counter
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


module Ring_Counter(
    input Advance,
    input clk,
    output [3:0] v_out
    );
    wire [3:0] Q;
    wire not_q0, not_q3;
    assign not_q0 = ~Q[0];
    assign not_q3 = ~Q[3];
    
    FDRE #(.INIT(1'b0) ) ff_0 (.C(clk), .R(1'b0), .CE(Advance), .D(not_q3), .Q(Q[0]));
    FDRE #(.INIT(1'b0) ) ff_1 (.C(clk), .R(1'b0), .CE(Advance), .D(not_q0), .Q(Q[1]));
    FDRE #(.INIT(1'b0) ) ff_2 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[1]), .Q(Q[2]));
    FDRE #(.INIT(1'b0) ) ff_3 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[2]), .Q(Q[3]));
    
    assign v_out[3:1] = Q[3:1];
    assign v_out[0] = ~Q[0];
    
endmodule

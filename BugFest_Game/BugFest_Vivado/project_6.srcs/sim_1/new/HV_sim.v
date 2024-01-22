`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/22/2023 02:51:46 PM
// Design Name: 
// Module Name: HV_sim
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


module HV_sim();
    wire [14:0] H_out, V_out;
    wire end_out;
    reg clkin;
    HV_coords hv(.clk(clkin), .H(H_out), .V(V_out), .endFrame(end_out));
    
    initial
    begin
    #10
    forever
        #25
        clkin = ~clkin;
        
    end
    initial
    begin
    clkin = 0;
    end
endmodule

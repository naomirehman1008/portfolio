`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/23/2023 11:08:39 AM
// Design Name: 
// Module Name: topL6s23
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


module topL6S23(
    input btnU,
    input btnD,
    input btnC,
    input btnR,
    input btnL,
    input clkin,
    output [6:0] seg,
    output dp,
    output [3:0] an,
    output [3:0] vgaBlue,
    output [3:0] vgaRed,
    output [3:0] vgaGreen,
    output Vsync,
    output Hsync,
    input [15:0] sw,
    output [15:0] led
    );
    wire [14:0] Hout, Vout;
    wire activeHigh;
    wire FrameUpdate;
    wire clk, digsel;
    labVGA_clks not_so_slow (.clkin(clkin), .greset(btnR), .clk(clk), .digsel(digsel));
    
    HV_coords getCoords(.clk(clk), .H(Hout), .V(Vout));
    
    Syncs getSyncs(.H(Hout), .V(Vout), .clk(clkin), .Hsync(Hsync), .Vsync(Vsync), .Active(activeHigh), .Frame(FrameUpdate));
    
    //SET STUFF FOR CONSTRAINSTS
    //assign seg[6:0] = {7{1'b1}};
    //assign dp = 1'b1;
    //assign an[3:0] = {3{1'b1}};
    assign led = {16{1'b0}};
    //init signal
    wire init, ZERO;
    FDRE #(.INIT(1'b1) ) ff_0 (.C(clk), .R(1'b0), .CE(1'b1), .D(ZERO), .Q(init));
    assign ZERO = init & 1'b0;
    //component modules
    wire platformDW;
    wire [14:0] platformHupper0, platformHupper1, platformHupper2;
    wire [14:0] randOUT0, randOUT1, randOUT2;
    borderHCount platforms0(.clk(clk), .platformDW(platformDW), .Frame(FrameUpdate), .init(init), .Din(8'b10100100), .initBus(15'd201), .rand(randOUT0), .Hupper(platformHupper0));
    borderHCount platforms1(.clk(clk), .platformDW(platformDW), .Frame(FrameUpdate), .init(init), .Din(8'b00011001), .initBus(15'd501), .rand(randOUT1), .Hupper(platformHupper1));
    borderHCount platforms2(.clk(clk), .platformDW(platformDW), .Frame(FrameUpdate), .init(init), .Din(8'b11111101), .initBus(15'd801), .rand(randOUT2), .Hupper(platformHupper2));
    
    wire bugDW, slugUP, slugDW, slugleftEN;
    wire [14:0] bugH, bugV, slugH, slugV;
    wire scoreINC, bugFLASH, timeUP, slugFLASH;
    
    bug_mod bug(.clk(clk), .Frame(FrameUpdate), .INIT(init), .load(scoreINC), .Din(8'b10100100), .DW(bugDW), .bugH(bugH), .bugV(bugV));
    slug_mod slug(.clk(clk), .Frame(FrameUpdate), .UP(slugDW), .DW(slugUP), .leftEN(slugleftEN), .INIT(init), .SlugH(slugH), .SlugV(slugV));
    
    //GAME COMPONENTS
    wire POND, BORDER, PLATFORM, inBORDER, BUG, SLUG;
    wire [1:0] FLASH;
    assign BORDER = ((15'd0 <= Hout) & (15'd7 >= Hout)) 
        | ((15'd632 <= Hout) & (15'd639 >= Hout))
        | ((15'd0 <= Vout) & (15'd7 >= Vout)) 
        | ((15'd472 <= Vout) & (15'd479 >= Vout));
        
    assign POND = (((15'd400 <= Vout) & (15'd471 >= Vout)) & ((15'd8 <= Hout) & (15'd631 >= Hout))) & inBORDER;
    
    assign PLATFORM = (((15'd200 <= Vout) & (15'd207 >= Vout)) & (((platformHupper0 <= Hout + randOUT0) & (platformHupper0 >= Hout))
     | ((platformHupper1 <= Hout + randOUT1) & (platformHupper1 >= Hout)) |
    ((platformHupper2 <= Hout + randOUT2) & (platformHupper2 >= Hout)))) & inBORDER;
    
    assign inBORDER = ((15'd8 <= Hout) & (15'd631 >= Hout) & (15'd8 <= Vout) & (15'd471 >= Vout));
    
    assign BUG = ((bugV <= Vout + 15'd8) & (bugV>= Vout) & (bugH <= Hout + 15'd8) & (bugH >= Hout)) & inBORDER & (~bugFLASH | (bugFLASH & FLASH[0]));
    assign SLUG = ((slugV <= Vout + 15'd16) & (slugV >= Vout) & (slugH <= Hout + 15'd16) & (slugH >= Hout)) & inBORDER & (~slugFLASH | (slugFLASH & FLASH[0]));
    
    //STATE MACHINE 
    wire slugpond, slugbug, slugplatformtopbottom, slugplatformside, slugborder, slugtop;
    
    assign slugpond = 15'd400 <= slugV;
    assign slugbug = (slugH + 15'd8 >= bugH) & (slugH <= bugH + 15'd8) & (slugV + 15'd8 >= bugV) & (slugV <= bugV + 15'd8); //sus
    assign slugplatformtopbottom = (((slugV >= 15'd200) & (slugV < 15'd207) | (slugV <= 15'd207 + 15'd16) & (slugV > (15'd200 + 15'd16)))) & 
    (((platformHupper0 <= slugH + randOUT0) & (platformHupper0  + 15'd16 >= slugH)) |
    ((platformHupper1 <= slugH + randOUT1) & (platformHupper1  + 15'd16 >= slugH)) | //doesn't take in to account the length of the slug!!!
    ((platformHupper2 <= slugH + randOUT0) & (platformHupper2 + 15'd16 >= slugH)));
    
    assign slugplatformside = (15'd207 + 15'd16 >= slugV) & (15'd200 <= slugV) & ((slugH + randOUT0 == platformHupper0) | (slugH + randOUT1 == platformHupper1) | (slugH + randOUT2 == platformHupper2));
    assign slugborder = 15'd7 + 15'd16 >= slugH;
    assign slugtop = 15'd7 + 15'd16 >= slugV;
    

    bugfest_statemachine myState(.clk(clk), .timeUP(timeUP), .btnU(btnU), .btnC(btnC), .SlugPond(slugpond), .SlugBug(slugbug), .SlugPlatformTopBottom(slugplatformtopbottom), .SlugPlatformSide(slugplatformside), .SlugBorder(slugborder), .SlugTop(slugtop), .slugUP(slugUP), .slugDN(slugDW), .slugleftEN(slugleftEN), .slugFLASH(slugFLASH), .bugDW(bugDW), .bugFLASH(bugFLASH), .platformDW(platformDW), .scoreINC(scoreINC));
        
    //COLOR ASSIGNMENTS:
    
    assign vgaRed[0] = ((1'b0) | PLATFORM | SLUG) & activeHigh;
    assign vgaRed[1] = ((1'b0) | PLATFORM | SLUG) & activeHigh;
    assign vgaRed[2] = ((1'b0) | PLATFORM | SLUG) & activeHigh;
    assign vgaRed[3] = (1'b0 | PLATFORM | SLUG) & activeHigh;
    
    assign vgaBlue[0] = (POND | BORDER | PLATFORM) & activeHigh;
    assign vgaBlue[1] = (POND | BORDER | PLATFORM) & activeHigh;
    assign vgaBlue[2] = (POND | BORDER | PLATFORM) & activeHigh;
    assign vgaBlue[3] = (BORDER) & activeHigh;
    
    assign vgaGreen[0] = (POND | SLUG | BUG) & activeHigh;
    assign vgaGreen[1] = (POND | SLUG | BUG) & activeHigh;
    assign vgaGreen[2] = ((1'b0 | SLUG | BUG)) & activeHigh;
    assign vgaGreen[3] = (1'b0 | SLUG | BUG) & activeHigh;
    
    //counters
    wire [14:0] SCORE, counter1_out, counter2_out;
    wire qsec;
 
    counterUD15L score(.clk(clk), .Up(scoreINC), .Dw(1'b0), .LD(1'b0), .Din(), .Q(SCORE));
    counterUD15L twoSeconds(.clk(clk), .Up(1'b0), .Dw(bugFLASH & FrameUpdate), .LD(scoreINC | init), .Din(15'd120), .Q(counter1_out), .DTC());
    counterUD15L qsec_sig(.clk(clk), .Up(1'b0), .Dw(FrameUpdate), .LD(qsec | init), .Din(15'd16), .Q(counter2_out), .DTC());
    
    assign timeUP = &(~(counter1_out));
    assign qsec = &(~(counter2_out));
    //Flashing
    
    FDRE #(.INIT(1'b1) ) ff_zero (.C(clk), .R(1'b0), .CE(qsec), .D(FLASH[1]), .Q(FLASH[0]));
    FDRE #(.INIT(1'b0) ) ff_1 (.C(clk), .R(1'b0), .CE(qsec), .D(FLASH[0]), .Q(FLASH[1]));
    
    //seven segment stuff
    wire [3:0] sel, n_vec;
    Ring_Counter ringCounter(.Advance(digsel), .clk(clk), .v_out(sel));
    Selector selector(.N({{8{1'b0}}, SCORE[7:0]}), .sel(sel), .H(n_vec));
    hex7seg myseg(.n(n_vec), .seg(seg));
    assign an = ~sel;
    assign dp = 1'b1;

endmodule

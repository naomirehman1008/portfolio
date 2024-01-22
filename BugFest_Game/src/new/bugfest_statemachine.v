`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/29/2023 10:01:18 PM
// Design Name: 
// Module Name: bugfest_statemachine
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


module bugfest_statemachine(
    input clk,
    input timeUP,
    input btnU,
    input btnC,
    input SlugPond,
    input SlugBug,
    input SlugPlatformTopBottom,
    input SlugPlatformSide,
    input SlugBorder,
    input SlugTop,
    output slugUP,
    output slugDN,
    output slugleftEN,
    output slugFLASH,
    output bugDW,
    output bugFLASH,
    output platformDW,
    output scoreINC
    );
    
    wire CHILL, CHILLNOMOVE, PLAYGAME, ENDGAME, SCOREINC, BUGFLASH, SLUGNOMOVE, SLUGHITPLATFORM;
    wire nextCHILL, nextCHILLNOMOVE, nextPLAYGAME, nextENDGAME, nextSCOREINC, nextBUGFLASH, nextSLUGNOMOVE, nextSLUGHITPLATFORM;
    wire reset, enable;
    assign reset = 1'b0;
    assign enable = 1'b1;
    FDRE #(.INIT(1'b1) ) ff_chill (.C(clk), .R(reset), .CE(enable), .D(nextCHILL), .Q(CHILL));
    FDRE #(.INIT(1'b1) ) ff_chillNoMove (.C(clk), .R(reset), .CE(enable), .D(nextCHILLNOMOVE), .Q(CHILLNOMOVE));
    FDRE #(.INIT(1'b0) ) ff_playGame (.C(clk), .R(reset), .CE(enable), .D(nextPLAYGAME), .Q(PLAYGAME));
    FDRE #(.INIT(1'b0) ) ff_endGame (.C(clk), .R(reset), .CE(enable), .D(nextENDGAME), .Q(ENDGAME));
    FDRE #(.INIT(1'b0) ) ff_scoreINC (.C(clk), .R(reset), .CE(enable), .D(nextSCOREINC), .Q(SCOREINC));
    FDRE #(.INIT(1'b0) ) ff_bugFlash (.C(clk), .R(reset), .CE(enable), .D(nextBUGFLASH), .Q(BUGFLASH));
    FDRE #(.INIT(1'b0) ) ff_slugNoMove (.C(clk), .R(reset), .CE(enable), .D(nextSLUGNOMOVE), .Q(SLUGNOMOVE));
    FDRE #(.INIT(1'b0) ) ff_slugHitPlatform (.C(clk), .R(reset), .CE(enable), .D(nextSLUGHITPLATFORM), .Q(SLUGHITPLATFORM));
    
    assign nextCHILL = (CHILL & ~btnC) | (CHILLNOMOVE & ~SlugPlatformTopBottom & ~btnC);     
    assign nextCHILLNOMOVE = (CHILL) & SlugPlatformTopBottom;
    //SUS
    assign nextPLAYGAME = (PLAYGAME & ~(SlugPlatformSide) & ~(SlugBug) & ~(SlugPond) & ~(SlugBorder)) | (BUGFLASH & timeUP) | (CHILL & btnC) | (SLUGNOMOVE & ~(SlugPlatformTopBottom)) | (CHILLNOMOVE & btnC);
    assign nextENDGAME = (PLAYGAME & (SlugPond)) | ENDGAME | (SLUGHITPLATFORM & SlugBorder);
    assign nextSCOREINC = (PLAYGAME & (SlugBug));
    assign nextBUGFLASH = SCOREINC | (BUGFLASH & ~timeUP);
    assign nextSLUGNOMOVE = (PLAYGAME | BUGFLASH) & SlugPlatformTopBottom;
    assign nextSLUGHITPLATFORM = (PLAYGAME & SlugPlatformSide) | (SLUGHITPLATFORM & SlugPlatformSide & ~SlugBorder);
    
    assign slugUP = btnU & (CHILL | PLAYGAME | BUGFLASH) & ~SlugTop & ~SLUGNOMOVE & ~ENDGAME & ~CHILLNOMOVE;
    assign slugDN = ~btnU & (CHILL | PLAYGAME | BUGFLASH) & ~SLUGNOMOVE & ~ENDGAME & ~CHILLNOMOVE;
    assign slugleftEN = SLUGHITPLATFORM & ~ENDGAME;
    assign slugFLASH = ENDGAME;
    assign bugDW = (PLAYGAME | ENDGAME | SLUGNOMOVE | SLUGHITPLATFORM) & ~BUGFLASH;
    assign bugFLASH = BUGFLASH;
    assign platformDW = (PLAYGAME | SLUGNOMOVE | SLUGHITPLATFORM) & ~BUGFLASH;
    assign scoreINC = SCOREINC;
    
    
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/20/2024 10:44:38 AM
// Design Name: 
// Module Name: cpu_top
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
// ISA spec can be found at https://user.eng.umd.edu/~blj/risc/RiSC-isa.pdf
//////////////////////////////////////////////////////////////////////////////////


module cpu_top #(
    parameter DATA_WIDTH=16,
    parameter NUM_REGISTERS=8,
    parameter REG_ADDR_BITS=3,
    parameter OP_WIDTH=3 
)
(
    input clk,
    input [DATA_WIDTH-1:0] mem_data_in,
    output reg mem_read,
    output reg mem_write,
    output reg [DATA_WIDTH-1:0] mem_addr,
    output reg [DATA_WIDTH-1:0] mem_data_out
    );
    //REG TYPES
    reg [DATA_WIDTH-1:0] PC;
    reg [DATA_WIDTH-1:0] PC_next;
    reg [DATA_WIDTH-1:0] Inst;
    
    //WIRE TYPES
    wire Branch;
    wire RegWrite;
    wire ALUsrc;
    wire MemWrite;
    wire MemRead;
    wire Mem2Reg;
    wire Zero;
    
    wire [DATA_WIDTH-1:0] ALUout;
    
    wire [DATA_WIDTH-1:0] rs;
    wire [DATA_WIDTH-1:0] rt;
    
    reg [DATA_WIDTH-1:0] A;
    reg [DATA_WIDTH-1:0] B;
    reg [DATA_WIDTH-1:0] res_data;
    
    wire [REG_ADDR_BITS-1:0] rs_addr;
    wire [REG_ADDR_BITS-1:0] rt_addr;
    wire [REG_ADDR_BITS-1:0] rd_addr;
    wire [OP_WIDTH-1:0] op;
    wire [6:0] s_imdt;
    signed [9:0] u_imdt;
    
    //op field
    assign op = Inst[15:13];
    //immediate field
    assign s_imdt = Inst[6:0]; //signed immediate
    assign u_imdt = Inst[9:0]; //unsigned immediat
    assign rd_addr = Inst[12:10];
    assign rs_addr = Inst[9:7];
    assign rt_addr = Inst[2:0]; //I think this is wrong, check ISA
    
    //Program Counter
    initial begin
        PC <= 0;
    end
    always @(posedge clk) begin
        if(Branch && Zero) begin
        //plus 1?
            PC_next <= PC + (imdt << 1); //word aligned
        end
        else begin
            PC_next <= PC + 2;
        end
        //
        mem_addr <= PC;
        //this is a tad bit sus
        mem_read <= 1;
        Inst <= mem_data_in;
        PC <= PC_next;
    end
    
    general_register_file #(.DATA_WIDTH(DATA_WIDTH), 
        .NUM_REGISTERS(NUM_REGISTERS), 
        .REG_ADDR_BITS(REG_ADDR_BITS)) GRF (.clk(clk),
        .wrreg(RegWrite),
        .wrdata(res_data),
        .wraddr(rd_addr),
        .raddr1(rs_addr),
        .raddr2(rt_addr),
        .rdata1(rs),
        .rdata2(rt));
        
    ALU #(.DATA_WIDTH(DATA_WIDTH),
        .NUM_REGISTERS(NUM_REGISTERS),
        .REG_ADDR_BITS(REG_ADDR_BITS),
        .OP_WIDTH(OP_WIDTH)
)
(
        .clk(clk),
        .ALUctl(op),
        .A(A),
        .B(B),
        .PC(PC),
        .ALUOut(ALUout),
        .zero(Zero)
    );
    
    //alu src mulitplexor
    always @(*) begin
        if(ALUsrc) begin
            B <= imdt;
        end else begin
            B <= rt;
        A <= rs;
        end
    end
    
    //result multiplexor
    always @(*) begin
        if (MemRead) begin
            res_data <= mem_data_in;
        end
        else begin
            res_data <= ALUout;
        end
    end 
        
    controlsig #(
        .OP_WIDTH(OP_WIDTH)
)
(
    .clk(clk),
    .op(op),
    .Branch(Branch),
    .RegWrite(RegWrite),
    .ALUsrc(ALUsrc),
    .MemWrite(MemWrite),
    .MemRead(MemRead),
    .Mem2Reg(Mem2Reg)
    );
        
endmodule

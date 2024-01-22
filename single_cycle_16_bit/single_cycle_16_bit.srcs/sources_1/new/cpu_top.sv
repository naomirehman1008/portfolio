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
    input [DATA_WIDTH-1:0] inst_in,
    output reg [DATA_WIDTH-1:0] inst_addr,
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
    wire U_Imdt;
    
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
    logic signed [6:0] s_imdt; //ensure propper sign extension
    wire [9:0] u_imdt;
    
    //op field
    assign op = Inst[15:13];
    //immediate field
    assign s_imdt = Inst[6:0]; //signed immediate
    assign u_imdt = Inst[9:0]; //unsigned immediat
    assign rd_addr = Inst[12:10];
    assign rs_addr = Inst[9:7];
    assign rt_addr = Inst[2:0]; //I think this is wrong, check ISA
    
    //register file
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
    .Mem2Reg(Mem2Reg),
    .U_Imdt(U_Imdt)
    );
    //Program Counter init
    initial begin
        PC <= 0;
    end
    
    //program counter control
    always @(posedge clk) begin
        if(Branch && Zero) begin
        //plus 1?
            PC_next <= PC + (s_imdt << 1); //word aligned
        end
        else begin
            PC_next <= PC + 2;
        end
        //
        inst_addr <= PC;
        Inst <= inst_in;
        PC <= PC_next;
    end
    
    //alu src mulitplexor
    always @(*) begin
        if(ALUsrc) begin
            if (U_Imdt) begin
                B <= u_imdt;
            end else begin
                B <= s_imdt;
            end
        end else begin
            B <= rt;
        A <= rs;
        end
    end
    
    //result multiplexor
    always @(*) begin
        if (Mem2Reg) begin
            res_data <= mem_data_in;
        end
        else begin
            res_data <= ALUout;
        end
    end 
        
    //read memory
    always @(*) begin
        if(MemRead) begin
            mem_read <= 1;
            mem_addr <= rs;
            mem_write <= 0;
        end else if (MemWrite) begin
            mem_write <= 1;
            mem_addr <= rs;
            mem_read <= 0;
        end else begin
            mem_read <= 0;
            mem_write <= 0;
        end
   end
endmodule

module conv_unit #(
    parameter DATA_WIDTH = 8
)
(
    input clk,
    input [2:0][2:0][DATA_WIDTH-1:0] input_data,
    output reg [7:0] output_data
    );
     
     always @(posedge clk) begin
        output_data <= input_data[0][1]
                + input_data[1][0] 
                - (input_data[1][1] << 2)
                +input_data[1][2]
                +input_data[2][1];      
     end
 endmodule


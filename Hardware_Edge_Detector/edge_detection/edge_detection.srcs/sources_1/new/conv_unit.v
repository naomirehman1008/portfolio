module conv_unit #(
    parameter DATA_WIDTH = 8
)
(
    input clk,
    input [7:0] input_data [8:0],
    output reg [7:0] output_data
    );
     
     initial begin
        output_data <= 0;
     end
     
     always @(posedge clk) begin
        output_data <= input_data[1] 
            + input_data[3] 
            - (input_data[4] << 2)
            +input_data[5]
            +input_data[7];
     end
 endmodule
    
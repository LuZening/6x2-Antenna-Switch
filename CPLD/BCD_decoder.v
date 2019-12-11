// BCD decoder 3bit
// HIGH effective
module BCD_decoder(I_bcd, O_bin);
input [2:0] I_bcd;
output reg [5:0] O_bin;
	always @(I_bcd)
	begin
		case (I_bcd)
			3'd0 : O_bin = 6'b0;
			3'd1 : O_bin = 6'b1;
			3'd2 : O_bin = 6'b10;
			3'd3 : O_bin = 6'b100;
			3'd4 : O_bin = 6'b1000;
			3'd5 : O_bin = 6'b10000;
			3'd6 : O_bin = 6'b100000;
			//3'd7 : O_bin = 6'b1000000;
			default : O_bin = 6'b0;
		endcase
	end
endmodule

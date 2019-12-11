// 7SEG display decoder
// I: BCD 3bit High effective ranging 0-7
// O: Low Effective 7SEG signal
module Seven_seg_decoder(I, I_collision, O); // input: BCD HIGH Effective, output: LOW effective
input [2:0] I;
input I_collision;
output reg [7:0] O;
always @ (*)
begin
	if(!I_collision)
	begin
		case (I)
			3'd0 : O = 8'b11000000;
			3'd1 : O = 8'b11111001;
			3'd2 : O = 8'b10100100;
			3'd3 : O = 8'b10110000;
			3'd4 : O = 8'b10011001;
			3'd5 : O = 8'b10010010;
			3'd6 : O = 8'b10000010;
			3'd7 : O = 8'b11111000;
			//3'd8 : O = 7'b0000000;
			//3'd9 : O = 7'b0010000;
			default : O = 8'b11111111;
		endcase
	end
	else
	begin
		O = 8'b11000110; // displays 'C' when collision detected
	end
end
endmodule
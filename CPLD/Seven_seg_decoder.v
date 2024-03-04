// 7SEG display decoder
// I: BCD 3bit High effective ranging 0-7
// O: Low Effective 7SEG signal
module Seven_seg_decoder(I, I_collision, O); // input: BCD HIGH Effective, output: LOW effective
input [3:0] I;
input I_collision;
output reg [7:0] O;
always @ (*)
begin
	if(!I_collision)
	begin
		case (I)
			4'd0 : O = 8'b11000000; // 0
			4'd1 : O = 8'b11111001;
			4'd2 : O = 8'b10100100;
			4'd3 : O = 8'b10110000;
			4'd4 : O = 8'b10011001;
			4'd5 : O = 8'b10010010;
			4'd6 : O = 8'b10000010;
			4'd7 : O = 8'b11111000;
			4'd8 : O = 8'b10000000; // 8
			4'd9 : O = 8'b10010000; // 9
			4'd10: O = 8'b01111111; // .
			4'b1111 : O = 8'b10111111; // -
			default : O = 8'b11111111; // no display
		endcase
	end
	else
	begin
		O = 8'b11000110; // displays 'C' when collision detected
	end
end
endmodule
// BCD encoder 3bit
module BCD_encoder(I_bin, O_bcd);
input [5:0] I_bin;
output reg [2:0] O_bcd;
always @ (I_bin)
begin
	casex (I_bin)
		6'b1xxxxx : O_bcd = 6;
		6'b01xxxx : O_bcd = 5;
		6'b001xxx : O_bcd = 4;
		6'b0001xx : O_bcd = 3;
		6'b00001x : O_bcd = 2;
		6'b000001 : O_bcd = 1;
		default : O_bcd = 0;
	endcase
end
endmodule
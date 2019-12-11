// input interlock
// I_A: Switch A BCD input HIGH
// I_B: Switch B BCD input HIGH
// O_A: Switch A 6bit output HIGH
// O_B: Swtich B 6bit output HIGH
// O_collision: Collision detecting signal HIGH 
module Interlock(I_A, I_B, O_A, O_B, O_collision); //  I_A,I_B: BCD; O_A, O_B Binary collision: HIGH, collision occurs

	input [2:0] I_A;
	input [2:0] I_B;
	output [5:0]  O_A;
	output [5:0] O_B;
	output O_collision;
	wire [5:0] W_bin_A;
	wire [5:0] W_bin_B;
	assign O_collision = (I_A == I_B && I_A != 0)?(1):(0);


	BCD_decoder u_Interlock_BCD_decoder_A (.I_bcd(I_A), .O_bin(W_bin_A));
	BCD_decoder u_Interlock_BCD_decoder_B (.I_bcd(I_B), .O_bin(W_bin_B));
	
	assign O_A = (O_collision)?(0):(W_bin_A);
	assign O_B = (O_collision)?(0):(W_bin_B);

endmodule

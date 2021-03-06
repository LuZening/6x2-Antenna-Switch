// input MUX
// select between remote and local inputs (BCD in, BCD out)
module I_MUX(I_local_A, I_local_B, I_remote_A, I_remote_B, I_remote, O_A, O_B);
	input [2:0] I_local_A;
	input [2:0] I_local_B;
	input [2:0] I_remote_A;
	input [2:0] I_remote_B;
	input I_remote;
	output [2:0] O_A;
	output [2:0] O_B;
	
	assign O_A = (I_remote)?(I_remote_A):(I_local_A);
	assign O_B = (I_remote)?(I_remote_B):(I_local_B);
endmodule

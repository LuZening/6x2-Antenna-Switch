// Copyright (C) 2018  Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License 
// Subscription Agreement, the Intel Quartus Prime License Agreement,
// the Intel FPGA IP License Agreement, or other applicable license
// agreement, including, without limitation, that your use is for
// the sole purpose of programming logic devices manufactured by
// Intel and sold by Intel or its authorized distributors.  Please
// refer to the applicable agreement for further details.

module Antenna_Switch_Interlock_pin
(
// {ALTERA_ARGS_BEGIN} DO NOT REMOVE THIS LINE!

	BCD1,
	BCD2,
	BCDM1,
	BCDM2,
	LOAD,
	REMOTE,
	RST_EXT,
	SEG1,
	SEG1_en,
	SEG2,
	SEG2_en,
	SEL1,
	SEL2,
	SW1,
	SW2
// {ALTERA_ARGS_END} DO NOT REMOVE THIS LINE!

);

// {ALTERA_IO_BEGIN} DO NOT REMOVE THIS LINE!
input	[2:0]	BCD1;
input	[2:0]	BCD2;
output	[2:0]	BCDM1;
output	[2:0]	BCDM2;
input			LOAD;
input			REMOTE;
input			RST_EXT;
output	[7:0]	SEG1;
output			SEG1_en;
output	[7:0]	SEG2;
output			SEG2_en;
output	[5:0]	SEL1;
output	[5:0]	SEL2;
input	[5:0]	SW1;
input	[5:0]	SW2;

// {ALTERA_IO_END} DO NOT REMOVE THIS LINE!
// {ALTERA_MODULE_BEGIN} DO NOT REMOVE THIS LINE!
	wire [2:0] W_SW1_encoded;
	wire [2:0] W_SW2_encoded;
	wire [2:0] W_IMUX_A;
	wire [2:0] W_IMUX_B;
	wire [5:0] W_Interlock_A;
	wire [5:0] W_Interlock_B;
	wire [2:0] W_Interlock_BCD_A;
	wire [2:0] W_Interlock_BCD_B; // Interlock outputs encoded to BCD
	wire W_collision;
	wire [5:0] W_latched_A;
	wire [5:0] W_latched_B;
	wire W_diff; // Signal if interlock outputs and latched outputs are different
	wire [2:0] W_Disp_A;
	wire [2:0] W_Disp_B;
	wire [2:0] W_BCDM1;
	wire [2:0] W_BCDM2;

	BCD_encoder u_BCD_enc_SW1(.I_bin(~SW1), .O_bcd(W_SW1_encoded));
	BCD_encoder u_BCD_enc_SW2(.I_bin(~SW2), .O_bcd(W_SW2_encoded));
	I_MUX u_IMUX(.I_local_A(W_SW1_encoded), .I_local_B(W_SW2_encoded), .I_remote_A(~BCD1), .I_remote_B(~BCD2), .I_remote(~REMOTE), 
						.O_A(W_IMUX_A), .O_B(W_IMUX_B));
	Interlock u_Interlock(.I_A(W_IMUX_A), .I_B(W_IMUX_B), .O_A(W_Interlock_A), .O_B(W_Interlock_B), .O_collision(W_collision));
	// encode interlock outputs to BCD
	BCD_encoder u_BCD_enc_I1 (.I_bin(W_Interlock_A), .O_bcd(W_Interlock_BCD_A));
	BCD_encoder u_BCD_enc_I2 (.I_bin(W_Interlock_B), .O_bcd(W_Interlock_BCD_B));
	// Latch at the final output
	Out_latch u_Olatch(.I_A(W_Interlock_A), .I_B(W_Interlock_B), .I_load(LOAD), .I_remote(~REMOTE), .I_collision(W_collision), .O_A(W_latched_A), .O_B(W_latched_B));
	assign SEL1 = ~W_latched_A; // LOW effective
	assign SEL2 = ~W_latched_B; // LOW effective
	BCD_encoder u_BCD_enc_M1 (.I_bin(W_latched_A), .O_bcd(W_BCDM1)); // LOW effective
	BCD_encoder u_BCD_enc_M2 (.I_bin(W_latched_B), .O_bcd(W_BCDM2)); // LOW effective
	assign BCDM1 = ~W_BCDM1;
	assign BCDM2 = ~W_BCDM2;
	assign W_diff = ((W_BCDM1 != W_Interlock_BCD_A) || (W_BCDM2 != W_Interlock_BCD_B)); // generate the signal of input changes
	assign W_Disp_A = (W_diff)?(W_Interlock_BCD_A):(W_BCDM1);
	assign W_Disp_B = (W_diff)?(W_Interlock_BCD_B):(W_BCDM2);
	Seven_seg_decoder u_7SEG_A (.I(W_Disp_A), .I_collision(W_collision), .O(SEG1[6:0]));
	Seven_seg_decoder u_7SEG_B (.I(W_Disp_B), .I_collision(W_collision), .O(SEG2[6:0]));
	assign SEG1[7] = ~W_diff;
	assign SEG2[7] = ~W_diff;
	wire W_diff_SEG1_en;
	wire W_diff_SEG2_en;
	reg R_Disp_off = 0;
	Diff u_Diff(.I_A1(W_IMUX_A), .I_B1(W_IMUX_B), .I_A2(W_BCDM1), .I_B2(W_BCDM2), .O_segA(W_diff_SEG1_en), .O_segB(W_diff_SEG2_en));
	always @ (negedge LOAD)
	begin
		R_Disp_off = 0;
		if(!REMOTE)
		begin
			R_Disp_off = 1;
		end
	end
	assign SEG1_en = ~((W_diff || !R_Disp_off) && W_diff_SEG1_en);
	assign SEG2_en = ~((W_diff || !R_Disp_off) && W_diff_SEG2_en);
	// {ALTERA_MODULE_END} DO NOT REMOVE THIS LINE!
endmodule

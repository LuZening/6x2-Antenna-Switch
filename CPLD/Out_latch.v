// output D-latch
// I_A: Switch A Input 6bit HIGH effective
// I_B: Switch B Input 6bit HIGH effective
// I_load: HIGH effective load signal
// I_remote: HIGH effective remote enabling signal
// I_collision: HIGH effective collision signal
// O_A: HIGH effective Switch A output
// O_B: HIGH effective Switch B output
module Out_latch(I_A, I_B, I_load, I_remote, I_collision, O_A, O_B);
input [5:0] I_A;
input [5:0] I_B;
input I_load;
input I_remote;
input I_collision;
output [5:0] O_A;
reg [5:0] reg_A = 6'b0;
output [5:0] O_B;
reg [5:0] reg_B = 6'b0;



always @ (posedge I_load) // load inputs to registors at fall edge of I_load
begin
	if(!I_collision && !I_remote)
	begin
		reg_A <= I_A;
		reg_B <= I_B;
	end
end

assign O_A = (I_remote)?(I_A):(reg_A);
assign O_B = (I_remote)?(I_B):(reg_B); // bypass registered outputs if remote is enabled

endmodule

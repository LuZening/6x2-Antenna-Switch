// change detector
module Diff(I_A1, I_B1, I_A2, I_B2 ,O_segA, O_segB); // segA, segB HIGH EFFECTIVE
input [2:0] I_A1;
input [2:0] I_A2;
input [2:0] I_B1;
input [2:0] I_B2;
output reg O_segA;
output reg O_segB;
always @(*)
begin
	if(I_A1 != I_A2 && I_B1 == I_B2)
	begin
		O_segA = 1;
		O_segB = 0;
	end
	else if(I_A1 == I_A2 && I_B1 != I_B2)
	begin
		O_segA = 0;
		O_segB = 1;
	end
	else // all display when no difference between signals
	begin
		O_segA = 1;
		O_segB = 1;
	end
end
endmodule
module MouseLatch(mouse_x, mouse_y, clock, enable, cur_x, cur_y);
	output reg [9:0] cur_x;
	output reg [9:0] cur_y;
	input enable;
	input clock;
	input [9:0] mouse_x;
	input [9:0] mouse_y;
	
	always @(posedge clock) begin
		if (enable) begin
			cur_x = mouse_x;
			cur_y = mouse_y;
		end
	end
endmodule
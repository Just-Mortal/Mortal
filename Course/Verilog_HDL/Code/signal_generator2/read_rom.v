module read_rom(
	input clk,
	input [13:0] addr,
	
	output reg [7:0] q);

	reg[7:0] rom[16383:0];

	initial begin
		$readmemh("wave.dat", rom);
	end

	always @(posedge clk) begin
		q <= rom[addr];
	end

endmodule

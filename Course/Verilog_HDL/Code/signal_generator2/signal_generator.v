`timescale 1 ps/ 1 ps

module signal_generator(
	input reset,
	input sys_clk,
	input [1:0] switch,
	input freq_add,
	input freq_dec,
	
	output vga_clk,
	output [7:0] wave_out);

	assign vga_clk = sys_clk;

	wire [11:0] wave_address;
	wire [11:0] freq_word;
	wire [11:0] phase_word;
	wire [13:0] rom_address;

	key_detection key_int(
		 .reset(reset),
		 .freq_add(freq_add),
		 .freq_dec(freq_dec),
		 
		 .freq_word(freq_word)
	);
	
	address_calculation address_int(
		.reset(reset),
		.sys_clk(sys_clk),
		.switch(switch),
		.freq_word(freq_word),
		 
		.wave_out(wave_out)
	);

endmodule

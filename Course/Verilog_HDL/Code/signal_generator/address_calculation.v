module address_calculation(
	input reset,
	input sys_clk,
	input [1:0] switch,
	
	output [7:0] wave_out);
	
	reg [11:0] freq_word;
	reg [11:0] phase_word;
	initial begin
		freq_word=12'd8;
	end
	 
	wire [13:0] rom_address;
	reg [11:0] freq_word_reg;
	reg [11:0] phase_word_reg;
	reg [11:0] phase_adder;
	reg [11:0] wave_address;

	always @(posedge sys_clk or negedge reset) begin
		if (reset == 0) begin
			freq_word_reg	<= 12'h000;
			phase_word_reg <= 12'h000;
			phase_adder		<= 12'h000;
			wave_address	<= 12'h000;
		end
		else begin
			freq_word_reg	<= freq_word;
			phase_word_reg <= phase_word;
			phase_adder		<= phase_adder + freq_word_reg;
			wave_address 	<= phase_adder + phase_word_reg; 
		end
	end

	assign rom_address = {switch, wave_address};

	read_rom dds_rom_int(
		.clk(sys_clk),
		.addr(rom_address),
		
		.q(wave_out)
	);
 
endmodule

`timescale 1 ps/ 1 ps

module dds(
    input sys_clk,
    input sys_rst_n,
	 input [1:0] switch,
	 input freq_switch,
	 input freq_switch_sub,
	 
	 output dds_clk,
    output [7:0] wave_out
);
	assign dds_clk=sys_clk;
	reg [11:0] freq_word;
	reg [11:0] phase_word;
	
	initial begin
		freq_word=12'd8;
	end
	 
	wire [13:0] rom_address_switch;
	reg [11:0] freq_word_reg;
	reg [11:0] phase_word_reg;
	reg [11:0] phase_adder;//相位累加器
	reg [11:0] rom_address;//存储深度2^10
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (sys_rst_n ==1'b0) begin
			freq_word_reg <= 12'h000;
		end
		else begin
			freq_word_reg <= freq_word;
		end
	end
	
	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (sys_rst_n ==1'b0) begin
			phase_word_reg <= 12'h000;
		end
		else begin
			phase_word_reg <= phase_word;
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (sys_rst_n ==1'b0) begin
			phase_adder <= 12'h000;
		end
		else begin
			phase_adder <= phase_adder + freq_word_reg; //对频率控制字进行累加      
		end
	end

	always @(posedge sys_clk or negedge sys_rst_n) begin
		if (sys_rst_n ==1'b0) begin
			rom_address <= 12'd0;
		end
		else begin
			rom_address <= phase_adder + phase_word_reg; 
		end
	end
	
	always @(negedge freq_switch or negedge sys_rst_n)begin
		if(sys_rst_n==0)begin
			freq_word=12'd8;
		end
		else begin
			freq_word<=freq_word+12'd4;
		end
	end

	assign rom_address_switch={switch,rom_address};

	rom dds_rom(
	.clk(sys_clk),
	.addr(rom_address_switch),
	.q(wave_out)
	);
 
endmodule

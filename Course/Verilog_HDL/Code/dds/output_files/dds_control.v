module dds_control(
	input sys_clk,
	input sys_rst_n,
	input [1:0] wave_switch,
	input freq_switch,
	output reg [1:0] switch
	output [11:0] freq_word
);
	always@(posedge sys_clk or negedge sys_rst_n)begin
		if(sys_rst_n==0)begin
			switch<=2'b00;
		else begin
			switch<=wave_switch;
		end
	end
	
	always@(posedge sys_clk or negedge sys_rst_n )begin
		if(sys_rst_n==0)begin
			freq_word<=12'd8;
		end
	end
	always@(negedge freq_switch)begin
		freq_word<=freq_word+12'd8;
	end
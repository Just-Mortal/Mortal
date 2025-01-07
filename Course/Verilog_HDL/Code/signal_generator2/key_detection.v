module key_detection(
    input reset,
	 input freq_add,
	 input freq_dec,
	 
    output reg [11:0] freq_word);
	
	initial begin
		freq_word=12'd8;
	end
	
	always @(negedge freq_add or negedge freq_dec or negedge reset) begin
		if (reset == 1'b0) begin
			freq_word <= 12'd8;
		end
		else begin
			if (freq_add == 1'b0) begin
				freq_word <= freq_word + 12'd4;
			end
			else if (freq_dec == 1'b0) begin
				if (freq_word > 12'd8) begin
					freq_word <= freq_word - 12'd4;
            end
            else begin
					freq_word <= 12'd4;
            end
			end
		end
	end

endmodule

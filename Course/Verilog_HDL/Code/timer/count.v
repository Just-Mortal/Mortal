module count(reset, clk, clk_out);
    input reset, clk;
    output clk_out;

    reg [31:0] cnt;
    reg clk_out;

    always @(posedge clk)
    begin
        if (reset)
            cnt <= 0;
        else if (cnt == 24999999)
        begin
            cnt <= 0;
            clk_out <= ~clk_out;
        end
        else
            cnt <= cnt + 1;
    end
endmodule

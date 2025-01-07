module clock(reset, clk, out_sec_l, out_sec_h, out_min_l, out_min_h, out_hour);
    input reset, clk;
    output [6:0] out_sec_l, out_sec_h, out_min_l, out_min_h, out_hour;

    wire [6:0] out_sec_l, out_sec_h, out_min_l, out_min_h, out_hour;
    wire [7:0] sec, min;
    reg [3:0] hour;
    wire sec_clk, min_clk, hour_clk;

    count sec_count(reset, clk, sec_clk);
    count60 sec2min(reset, sec_clk, sec, min_clk);
    count60 min2hour(reset, min_clk, min, hour_clk);

    always @(negedge hour_clk)
    begin
        if (reset)
            hour[3:0] <= 0;
        else
            hour[3:0] <= hour[3:0] + 1;
    end

    show show_sec_l(out_sec_l, sec[3:0]);
    show show_sec_h(out_sec_h, sec[7:4]);
    show show_min_l(out_min_l, min[3:0]);
    show show_min_h(out_min_h, min[7:4]);
    show show_hour(out_hour, hour[3:0]);
endmodule

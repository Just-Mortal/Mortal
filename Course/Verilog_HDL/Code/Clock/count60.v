module count60(reset, clk, qout, cout);
    input reset, clk;
    output cout;
    output [7:0] qout;

    reg [7:0] qout;
    reg cout;

    always @(posedge clk or posedge reset)
    begin
        if (reset)
            qout <= 0;
        else
        begin
            if (qout[3:0] == 9)
            begin
                qout[3:0] <= 0;
                if (qout[7:4] == 5)
                    qout[7:4] <= 0;
                else
                    qout[7:4] <= qout[7:4] + 1;
            end
            else
                qout[3:0] <= qout[3:0] + 1;

            cout = (qout == 8'h59) ? 1 : 0;
        end
    end
endmodule

module show(decodeout, indec);
    input [3:0] indec;
    output [6:0] decodeout;

    reg [6:0] decodeout;

    always @(indec)
    begin
        case (indec)
            4'd0: decodeout = 7'b1000000;
            4'd1: decodeout = 7'b1111001;
            4'd2: decodeout = 7'b0100100;
            4'd3: decodeout = 7'b0110000;
            4'd4: decodeout = 7'b0011001;
            4'd5: decodeout = 7'b0010010;
            4'd6: decodeout = 7'b0000010;
            4'd7: decodeout = 7'b1111000;
            4'd8: decodeout = 7'b0000000;
            4'd9: decodeout = 7'b0010000;
        endcase
    end
endmodule

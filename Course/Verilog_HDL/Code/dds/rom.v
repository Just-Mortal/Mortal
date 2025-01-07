`timescale 1 ps/ 1 ps
module rom (
    input wire clk, // 时钟信号
    input wire [13:0] addr, // 12位地址输入
    output reg [7:0] q // 8位数据输出
);


reg [7:0] rom[16383:0];

initial begin
    // 读取MIF文件并初始化ROM数组
    $readmemh("wave.dat", rom);
end

always @(posedge clk) begin
    // 根据地址异步读取数据
    q <= rom[addr];
end

endmodule

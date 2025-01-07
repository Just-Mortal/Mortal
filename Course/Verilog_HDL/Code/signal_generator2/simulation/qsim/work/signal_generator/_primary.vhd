library verilog;
use verilog.vl_types.all;
entity signal_generator is
    port(
        reset           : in     vl_logic;
        sys_clk         : in     vl_logic;
        switch          : in     vl_logic_vector(1 downto 0);
        freq_add        : in     vl_logic;
        freq_dec        : in     vl_logic;
        vga_clk         : out    vl_logic;
        wave_out        : out    vl_logic_vector(7 downto 0)
    );
end signal_generator;

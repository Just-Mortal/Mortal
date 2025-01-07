library verilog;
use verilog.vl_types.all;
entity dds is
    port(
        sys_clk         : in     vl_logic;
        sys_rst_n       : in     vl_logic;
        switch          : in     vl_logic_vector(1 downto 0);
        freq_switch     : in     vl_logic;
        freq_switch_sub : in     vl_logic;
        dds_clk         : out    vl_logic;
        wave_out        : out    vl_logic_vector(7 downto 0)
    );
end dds;

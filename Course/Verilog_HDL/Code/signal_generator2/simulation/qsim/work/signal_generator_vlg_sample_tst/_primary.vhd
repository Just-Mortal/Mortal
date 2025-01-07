library verilog;
use verilog.vl_types.all;
entity signal_generator_vlg_sample_tst is
    port(
        freq_add        : in     vl_logic;
        freq_dec        : in     vl_logic;
        reset           : in     vl_logic;
        switch          : in     vl_logic_vector(1 downto 0);
        sys_clk         : in     vl_logic;
        sampler_tx      : out    vl_logic
    );
end signal_generator_vlg_sample_tst;

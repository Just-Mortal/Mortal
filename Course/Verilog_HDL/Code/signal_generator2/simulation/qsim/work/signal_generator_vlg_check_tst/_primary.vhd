library verilog;
use verilog.vl_types.all;
entity signal_generator_vlg_check_tst is
    port(
        vga_clk         : in     vl_logic;
        wave_out        : in     vl_logic_vector(7 downto 0);
        sampler_rx      : in     vl_logic
    );
end signal_generator_vlg_check_tst;

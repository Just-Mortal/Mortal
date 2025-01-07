library verilog;
use verilog.vl_types.all;
entity rom is
    port(
        clk             : in     vl_logic;
        addr            : in     vl_logic_vector(13 downto 0);
        q               : out    vl_logic_vector(7 downto 0)
    );
end rom;

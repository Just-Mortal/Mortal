library verilog;
use verilog.vl_types.all;
entity cyclonev_asmiblock is
    generic(
        lpm_type        : string  := "cyclonev_asmiblock"
    );
    port(
        dclk            : in     vl_logic;
        sce             : in     vl_logic;
        oe              : in     vl_logic;
        data0out        : in     vl_logic;
        data1out        : in     vl_logic;
        data2out        : in     vl_logic;
        data3out        : in     vl_logic;
        data0oe         : in     vl_logic;
        data1oe         : in     vl_logic;
        data2oe         : in     vl_logic;
        data3oe         : in     vl_logic;
        data0in         : out    vl_logic;
        data1in         : out    vl_logic;
        data2in         : out    vl_logic;
        data3in         : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of lpm_type : constant is 1;
end cyclonev_asmiblock;
